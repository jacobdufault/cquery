// TODO: cleanup includes
#include "cache_manager.h"
#include "clang_complete.h"
#include "code_complete_cache.h"
#include "diagnostics_engine.h"
#include "file_consumer.h"
#include "import_manager.h"
#include "import_pipeline.h"
#include "include_complete.h"
#include "indexer.h"
#include "lex_utils.h"
#include "lru_cache.h"
#include "lsp_diagnostic.h"
#include "match.h"
#include "message_handler.h"
#include "options.h"
#include "platform.h"
#include "project.h"
#include "query.h"
#include "query_utils.h"
#include "queue_manager.h"
#include "recorder.h"
#include "semantic_highlight_symbol_cache.h"
#include "serializer.h"
#include "serializers/json.h"
#include "test.h"
#include "timer.h"
#include "timestamp_manager.h"
#include "work_thread.h"
#include "working_files.h"

#include <doctest/doctest.h>
#include <rapidjson/error/en.h>
#include <loguru.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// TODO: provide a feature like 'https://github.com/goldsborough/clang-expand',
// ie, a fully linear view of a function with inline function calls expanded.
// We can probably use vscode decorators to achieve it.

// TODO: implement ThreadPool type which monitors CPU usage / number of work
// items per second completed and scales up/down number of running threads.

std::string g_init_options;
Config* g_config;

namespace {

std::vector<std::string> kEmptyArgs;

// This function returns true if e2e timing should be displayed for the given
// MethodId.
bool ShouldDisplayMethodTiming(MethodType type) {
  return type != kMethodType_TextDocumentPublishDiagnostics &&
         type != kMethodType_CqueryPublishInactiveRegions &&
         type != kMethodType_Unknown;
}

void PrintHelp() {
  std::cout
      << R"help(cquery is a low-latency C/C++/Objective-C language server.

Mode:
  --check <path>
                Load the project and try to create a translation unit for path.
                This can be used to quickly test to see if your project
                configuration will work. The current directory is used as the
                project directory.
  --test-unit   Run unit tests.
  --test-index <opt_filter_path>
                Run index tests. opt_filter_path can be used to specify which
                test to run (ie, "foo" will run all tests which contain "foo"
                in the path). If not provided all tests are run.
  (default if no other mode is specified)
                Run as a language server over stdin and stdout

Other command line options:
  --init <initializationOptions>
                Override client provided initialization options
         https://github.com/cquery-project/cquery/wiki/Initialization-options
  --record <path>
                Writes stdin to <path>.in and stdout to <path>.out
  --log-file <path>
                Logging file for diagnostics
  --log-file-append <path>
                Like --log-file, but appending
  --log-all-to-stderr
                Write all log messages to STDERR.
  --wait-for-input
                Wait for an '[Enter]' before exiting
  --help        Print this help information.
  --ci          Prevents tests from prompting the user for input. Used for
                continuous integration so it can fail faster instead of timing
                out.
  --print-env   Print all environment variables cquery is running with.

See more on https://github.com/cquery-project/cquery/wiki
)help";
}

// Writes the environment to stdcerr.
void PrintEnvironment(const char** env) {
  while (*env) {
    std::cerr << *env << std::endl;
    ++env;
  }
}

struct Out_CqueryQueryDbStatus : public lsOutMessage<Out_CqueryQueryDbStatus> {
  struct Params {
    bool isActive = false;
  };
  std::string method = "$cquery/queryDbStatus";
  Params params;
};
MAKE_REFLECT_STRUCT(Out_CqueryQueryDbStatus::Params,
                    isActive);
MAKE_REFLECT_STRUCT(Out_CqueryQueryDbStatus,
                    jsonrpc,
                    method,
                    params);

void WriteQueryDbStatus(bool is_active) {
  if (!g_config->emitQueryDbBlocked)
    return;

  static bool last_status = false;
  if (is_active == last_status)
    return;
  last_status = is_active;
  Out_CqueryQueryDbStatus out;
  out.params.isActive = is_active;
  QueueManager::WriteStdout(out.method.c_str(), out);
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// QUERYDB MAIN ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool QueryDbMainLoop(QueryDatabase* db,
                     Project* project,
                     FileConsumerSharedState* file_consumer_shared,
                     ImportManager* import_manager,
                     ImportPipelineStatus* status,
                     TimestampManager* timestamp_manager,
                     SemanticHighlightSymbolCache* semantic_cache,
                     WorkingFiles* working_files,
                     ClangCompleteManager* clang_complete,
                     IncludeComplete* include_complete,
                     CodeCompleteCache* global_code_complete_cache,
                     CodeCompleteCache* non_global_code_complete_cache,
                     CodeCompleteCache* signature_cache) {
  auto* queue = QueueManager::instance();
  bool did_work = false;

  optional<std::unique_ptr<InMessage>> message = queue->for_querydb.TryDequeue(true /*priority*/);
  while (message) {
    did_work = true;

    bool found_handler = false;
    for (MessageHandler* handler : *MessageHandler::message_handlers) {
      if (handler->GetMethodType() == (*message)->GetMethodType()) {
        handler->Run(std::move(*message));
        found_handler = true;
        break;
      }
    }

    if (!found_handler) {
      LOG_S(FATAL) << "Exiting; no handler for " << (*message)->GetMethodType();
      exit(1);
    }

    message = queue->for_querydb.TryDequeue(true /*priority*/);
  }

  if (QueryDb_ImportMain(db, import_manager, status, semantic_cache,
                         working_files)) {
    did_work = true;
  }

  return did_work;
}

void RunQueryDbThread(const std::string& bin_name) {
  Project project;
  SemanticHighlightSymbolCache semantic_cache;
  WorkingFiles working_files;
  FileConsumerSharedState file_consumer_shared;
  DiagnosticsEngine diag_engine;

  ClangCompleteManager clang_complete(
      &project, &working_files,
      [&](std::string path, std::vector<lsDiagnostic> diagnostics) {
        diag_engine.Publish(&working_files, path, diagnostics);
      },
      [&](ClangTranslationUnit* tu, const std::vector<CXUnsavedFile>& unsaved,
          const std::string& path, const std::vector<std::string>& args) {
        IndexWithTuFromCodeCompletion(&file_consumer_shared, tu, unsaved, path,
                                      args);
      },
      [](lsRequestId id) {
        if (id.has_value()) {
          Out_Error out;
          out.id = id;
          out.error.code = lsErrorCodes::InternalError;
          out.error.message =
              "Dropping completion request; a newer request has come in that"
              "will be serviced instead. This is not an error.";
          QueueManager::WriteStdout(kMethodType_Unknown, out);
        }
      });

  IncludeComplete include_complete(&project);
  auto global_code_complete_cache = std::make_unique<CodeCompleteCache>();
  auto non_global_code_complete_cache = std::make_unique<CodeCompleteCache>();
  auto signature_cache = std::make_unique<CodeCompleteCache>();
  ImportManager import_manager;
  ImportPipelineStatus import_pipeline_status;
  TimestampManager timestamp_manager;
  QueryDatabase db;

  // Setup shared references.
  for (MessageHandler* handler : *MessageHandler::message_handlers) {
    handler->db = &db;
    handler->project = &project;
    handler->diag_engine = &diag_engine;
    handler->file_consumer_shared = &file_consumer_shared;
    handler->import_manager = &import_manager;
    handler->import_pipeline_status = &import_pipeline_status;
    handler->timestamp_manager = &timestamp_manager;
    handler->semantic_cache = &semantic_cache;
    handler->working_files = &working_files;
    handler->clang_complete = &clang_complete;
    handler->include_complete = &include_complete;
    handler->global_code_complete_cache = global_code_complete_cache.get();
    handler->non_global_code_complete_cache =
        non_global_code_complete_cache.get();
    handler->signature_cache = signature_cache.get();
  }

  // Run query db main loop.
  SetCurrentThreadName("querydb");
  while (true) {
    WriteQueryDbStatus(true);
    bool did_work = QueryDbMainLoop(
        &db, &project, &file_consumer_shared, &import_manager,
        &import_pipeline_status, &timestamp_manager, &semantic_cache,
        &working_files, &clang_complete, &include_complete,
        global_code_complete_cache.get(), non_global_code_complete_cache.get(),
        signature_cache.get());

    // Cleanup and free any unused memory.
    FreeUnusedMemory();

    if (!did_work) {
      WriteQueryDbStatus(false);
      auto* queue = QueueManager::instance();
      QueueManager::instance()->querydb_waiter->Wait(&queue->for_querydb,
                           &queue->do_id_map, &queue->on_indexed_for_querydb);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// STDIN MAIN //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Separate thread whose only job is to read from stdin and
// dispatch read commands to the actual indexer program. This
// cannot be done on the main thread because reading from std::cin
// blocks.
//
// |ipc| is connected to a server.
void LaunchStdinLoop(std::unordered_map<MethodType, Timer>* request_times) {
  // If flushing cin requires flushing cout there could be deadlocks in some
  // clients.
  std::cin.tie(nullptr);

  WorkThread::StartThread("stdin", [request_times]() {
    auto* queue = QueueManager::instance();
    while (true) {
      std::unique_ptr<InMessage> message;
      optional<std::string> err =
          MessageRegistry::instance()->ReadMessageFromStdin(&message);

      // Message parsing can fail if we don't recognize the method.
      if (err) {
        // The message may be partially deserialized.
        // Emit an error ResponseMessage if |id| is available.
        if (message) {
          lsRequestId id = message->GetRequestId();
          if (id.has_value()) {
            Out_Error out;
            out.id = id;
            out.error.code = lsErrorCodes::InvalidParams;
            out.error.message = std::move(*err);
            queue->WriteStdout(kMethodType_Unknown, out);
          }
        }
        continue;
      }

      // Cache |method_id| so we can access it after moving |message|.
      MethodType method_type = message->GetMethodType();
      (*request_times)[method_type] = Timer();

      queue->for_querydb.Enqueue(std::move(message), false /*priority*/);

      // If the message was to exit then querydb will take care of the actual
      // exit. Stop reading from stdin since it might be detached.
      if (method_type == kMethodType_Exit)
        break;
    }
  });
}

void LaunchStdoutThread(std::unordered_map<MethodType, Timer>* request_times) {
  WorkThread::StartThread("stdout", [=]() {
    auto* queue = QueueManager::instance();

    while (true) {
      Stdout_Request message = queue->for_stdout.Dequeue();

      if (ShouldDisplayMethodTiming(message.method)) {
        Timer time = (*request_times)[message.method];
        time.ResetAndPrint("[e2e] Running " + std::string(message.method));
      }

      RecordOutput(message.content);

      fwrite(message.content.c_str(), message.content.size(), 1, stdout);
      fflush(stdout);
    }
  });
}

void LanguageServerMain(const std::string& bin_name) {
  std::unordered_map<MethodType, Timer> request_times;

  LaunchStdinLoop(&request_times);

  // We run a dedicated thread for writing to stdout because there can be an
  // unknown number of delays when output information.
  LaunchStdoutThread(&request_times);

  // Start querydb which takes over this thread. The querydb will launch
  // indexer threads as needed.
  RunQueryDbThread(bin_name);
}

int main(int argc, char** argv, const char** env) {
  // `clang-format` will not output anything if PATH is not set.
  if (!getenv("PATH")) {
    LOG_S(WARNING) << "The \"PATH\" environment variable is not set. "
                   << "Formatting will not work.";
  }

  g_config = new Config();

  TraceMe();

  std::unordered_map<std::string, std::string> options =
      ParseOptions(argc, argv);

  // Setup logging ASAP.
  if (HasOption(options, "--log-file")) {
    loguru::add_file(options["--log-file"].c_str(), loguru::Truncate,
                     loguru::Verbosity_MAX);
  }
  if (HasOption(options, "--log-file-append")) {
    loguru::add_file(options["--log-file-append"].c_str(), loguru::Append,
                     loguru::Verbosity_MAX);
  }

  if (HasOption(options, "-h") || HasOption(options, "--help")) {
    PrintHelp();
    // Also emit doctest help if --test-unit is passed.
    if (!HasOption(options, "--test-unit"))
      return 0;
  }

  if (!HasOption(options, "--log-all-to-stderr"))
    loguru::g_stderr_verbosity = loguru::Verbosity_WARNING;

  loguru::g_preamble_date = false;
  loguru::g_preamble_time = false;
  loguru::g_preamble_verbose = false;
  loguru::g_flush_interval_ms = 0;
  loguru::init(argc, argv);

  QueueManager::Init();

  bool language_server = true;

  PlatformInit();
  IndexInit();

  if (HasOption(options, "--print-env"))
    PrintEnvironment(env);

  if (HasOption(options, "--record"))
    EnableRecording(options["--record"]);

  if (HasOption(options, "--check")) {
    loguru::g_stderr_verbosity = loguru::Verbosity_MAX;

    LOG_S(INFO) << "Running --check";

    optional<AbsolutePath> path = NormalizePath(options["--check"]);
    if (!path) {
      ABORT_S() << "Cannot find path \"" << options["--check"] << "\". Make "
                << "sure to pass a specific cc/cpp file, not a directory.";
    }
    optional<std::string> content = ReadContent(*path);
    if (!content || content->empty()) {
      ABORT_S() << "Cannot read file content at \"" << *path
                << "\". Make sure to pass a specific cc/cpp file, not a "
                << "directory.";
    }
    LOG_S(INFO) << "Using path " << *path;

    language_server = false;
    Project project;
    Config config;
    if (!GetDefaultResourceDirectory())
      ABORT_S() << "Cannot resolve resource directory";
    config.resourceDirectory = GetDefaultResourceDirectory()->path;
    project.Load(GetWorkingDirectory().path);
    Project::Entry entry = project.FindCompilationEntryForFile(path->path);
    LOG_S(INFO) << "Using arguments " << StringJoin(entry.args, " ");
    ClangSanityCheck(entry);
    return 0;
  }

  if (HasOption(options, "--test-unit")) {
    language_server = false;
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    int res = context.run();
    if (res != 0 || context.shouldExit())
      return res;
  }

  if (HasOption(options, "--test-index")) {
    language_server = false;
    if (!RunIndexTests(options["--test-index"], !HasOption(options, "--ci")))
      return 1;
  }

  if (language_server) {
    if (HasOption(options, "--init")) {
      // We check syntax error here but override client-side
      // initializationOptions in messages/initialize.cc
      g_init_options = options["--init"];
      rapidjson::Document reader;
      rapidjson::ParseResult ok = reader.Parse(g_init_options.c_str());
      if (!ok) {
        std::cerr << "Failed to parse --init as JSON: "
                  << rapidjson::GetParseError_En(ok.Code()) << " ("
                  << ok.Offset() << ")\n";
        return 1;
      }
      JsonReader json_reader{&reader};
      try {
        Config config;
        Reflect(json_reader, config);
      } catch (std::invalid_argument& e) {
        std::cerr << "Fail to parse --init "
                  << static_cast<JsonReader&>(json_reader).GetPath()
                  << ", expected " << e.what() << "\n";
        return 1;
      }
    }

    LanguageServerMain(argv[0]);
  }

  if (HasOption(options, "--wait-for-input")) {
    std::cerr << std::endl << "[Enter] to exit" << std::endl;
    getchar();
  }

  return 0;
}
