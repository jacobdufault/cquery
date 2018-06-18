#pragma once

#include "serializer.h"

#include <string>

/*
The language client plugin needs to send initialization options in the
`initialize` request to the cquery language server. The only required option is
`cacheDirectory`, which is where index files will be stored.

  {
    "initializationOptions": {
      "cacheDirectory": "/tmp/cquery"
    }
  }

If necessary, the command line option --init can be used to override
initialization options specified by the client. For example, in shell syntax:

  '--init={"index": {"comments": 2, "whitelist": ["."]}}'
*/
struct Config {
  // Root directory of the project. **Not available for configuration**
  std::string projectRoot;

  // If specified, this option overrides compile_commands.json and this
  // external command will be executed with an option |projectRoot|.
  // The initialization options will be provided as stdin.
  // The stdout of the command should be the JSON compilation database.
  std::string compilationDatabaseCommand;

  // Directory containing compile_commands.json.
  std::string compilationDatabaseDirectory;

  // Cache directory for indexed files.
  std::string cacheDirectory;

  // Cache serialization format.
  //
  // "json" generates `cacheDirectory/.../xxx.json` files which can be pretty
  // printed with jq.
  //
  // "msgpack" uses a compact binary serialization format (the underlying wire
  // format is [MessagePack](https://msgpack.org/index.html)) which typically
  // takes only 60% of the corresponding JSON size, but is difficult to inspect.
  // msgpack does not store map keys and you need to re-index whenever a struct
  // member has changed.
  SerializeFormat cacheFormat = SerializeFormat::Json;

  // Value to use for clang -resource-dir if not present in
  // compile_commands.json.
  //
  // cquery includes a resource directory, this should not need to be configured
  // unless you're using an esoteric configuration. Consider reporting a bug and
  // fixing upstream instead of configuring this.
  //
  // Example value: "/path/to/lib/clang/5.0.1/"
  std::string resourceDirectory;

  // Should system include directories be discovered/extracted by running
  // |clang++ -E -xc++ - -v|?
  //
  // System includes are necessary to resolve system/stl headers like <vector>.
  //
  // cquery will try a number of different compilers for system include
  // extraction. If there is a compiler mentioned in your project, that will be
  // used. Otherwise, cquery will use cquery-clang, and then the system-wide
  // clang++ or g++ will be used.
  bool discoverSystemIncludes = true;

  // Additional arguments to pass to clang.
  std::vector<std::string> extraClangArguments;

  // If true, cquery will send progress reports while indexing
  // How often should cquery send progress report messages?
  //  -1: never
  //   0: as often as possible
  //   xxx: at most every xxx milliseconds
  //
  // Empty progress reports (ie, idle) are delivered as often as they are
  // available and may exceed this value.
  //
  // This does not guarantee a progress report will be delivered every
  // interval; it could take significantly longer if cquery is completely idle.
  int progressReportFrequencyMs = 500;

  // If true, cquery will emit $cquery/queryDbStatus notifications whenever the
  // querydb thread is busy or idle.
  bool emitQueryDbBlocked = false;

  // If true, inactive regions notifications will be sent to the client.
  bool emitInactiveRegions = false;

  // If true, document links are reported for #include directives.
  bool showDocumentLinksOnIncludes = true;

  struct CodeLens {
    // Enables code lens on parameter and function variables.
    bool localVariables = true;
  };
  CodeLens codeLens;

  struct Completion {
    // If this is true and the client reports it can support snippets,
    // completion will include snippets. Set to false to disable snippets.
    bool enableSnippets = true;

    // Some completion UI, such as Emacs' completion-at-point and company-lsp,
    // display completion item label and detail side by side.
    // This does not look right, when you see things like:
    //     "foo" "int foo()"
    //     "bar" "void bar(int i = 0)"
    // When this option is enabled, the completion item label is very detailed,
    // it shows the full signature of the candidate.
    // The detail just contains the completion item parent context.
    // Also, in this mode, functions with default arguments,
    // generates one more item per default argument
    // so that the right function call can be selected.
    // That is, you get something like:
    //     "int foo()" "Foo"
    //     "void bar()" "Foo"
    //     "void bar(int i = 0)" "Foo"
    // Be wary, this is quickly quite verbose,
    // items can end up truncated by the UIs.
    bool detailedLabel = false;

    // On large projects, completion can take a long time. By default if cquery
    // receives multiple completion requests while completion is still running
    // it will only service the newest request. If this is set to false then all
    // completion requests will be serviced.
    bool dropOldRequests = true;

    // If true, filter and sort completion response. cquery filters and sorts
    // completions to try to be nicer to clients that can't handle big numbers
    // of completion candidates. This behaviour can be disabled by specifying
    // false for the option. This option is the most useful for LSP clients
    // that implement their own filtering and sorting logic.
    bool filterAndSort = true;

    // Maximum path length to show in completion results. Paths longer than this
    // will be elided with ".." put at the front. Set to 0 or a negative number
    // to disable eliding.
    int includeMaxPathSize = 30;

    // Whitelist that file paths will be tested against. If a file path does not
    // end in one of these values, it will not be considered for
    // auto-completion. An example value is { ".h", ".hpp" }
    //
    // This is significantly faster than using a regex.
    std::vector<std::string> includeSuffixWhitelist = {".h", ".hpp", ".hh"};

    // Regex patterns to match include completion candidates against. They
    // receive the absolute file path.
    //
    // For example, to hide all files in a /CACHE/ folder, use ".*/CACHE/.*"
    std::vector<std::string> includeBlacklist;
    std::vector<std::string> includeWhitelist;
  };
  Completion completion;

  struct Formatting {
    // Whether formatting should be enabled.
    bool enabled = true;
  };
  Formatting formatting;

  struct Diagnostics {
    // Like index.{whitelist,blacklist}, don't publish diagnostics to
    // blacklisted files.
    std::vector<std::string> blacklist;
    std::vector<std::string> whitelist;

    // How often should cquery publish diagnostics in completion?
    //  -1: never
    //   0: as often as possible
    //   xxx: at most every xxx milliseconds
    int frequencyMs = 0;

    // If true, diagnostics from a full document parse will be reported.
    bool onParse = true;
    // If true, diagnostics from typing will be reported.
    bool onType = true;
  };
  Diagnostics diagnostics;

  // Semantic highlighting
  struct Highlight {
    // Set to true to enable semantic highlighting.
    bool enabled = false;

    // Like index.{whitelist,blacklist}, don't publish semantic highlighting to
    // blacklisted files.
    std::vector<std::string> blacklist;
    std::vector<std::string> whitelist;
  };
  Highlight highlight;

  struct Index {
    // Attempt to convert calls of make* functions to constructors based on
    // heuristics.
    //
    // For example, this will show constructor calls for std::make_unique
    // invocations. Specifically, cquery will try to attribute a ctor call
    // whenever the function name starts with make (ignoring case).
    bool attributeMakeCallsToCtor = true;

    // If a translation unit's absolute path matches any EMCAScript regex in the
    // whitelist, or does not match any regex in the blacklist, it will be
    // indexed. To only index files in the whitelist, add ".*" to the blacklist.
    // `std::regex_search(path, regex, std::regex_constants::match_any)`
    //
    // Example: `ash/.*\.cc`
    std::vector<std::string> blacklist;
    std::vector<std::string> whitelist;

    // 0: none, 1: Doxygen, 2: all comments
    // Plugin support for clients:
    // - https://github.com/emacs-lsp/lsp-ui
    // - https://github.com/autozimu/LanguageClient-neovim/issues/224
    int comments = 2;

    // If false, the indexer will be disabled.
    bool enabled = true;

    // If true, project paths that were skipped by the whitelist/blacklist will
    // be logged.
    bool logSkippedPaths = false;

    // Number of indexer threads. If 0, 80% of cores are used.
    int threads = 0;
  };
  Index index;

  struct WorkspaceSymbol {
    // Maximum workspace search results.
    int maxNum = 1000;
    // If true, workspace search results will be dynamically rescored/reordered
    // as the search progresses. Some clients do their own ordering and assume
    // that the results stay sorted in the same order as the search progresses.
    bool sort = true;
  };
  WorkspaceSymbol workspaceSymbol;

  // Allow indexing on textDocument/didChange.
  // May be too slow for big projects, so it is off by default.
  bool enableIndexOnDidChange = false;

  struct Xref {
    // Maximum number of definition/reference/... results.
    unsigned int maxNum = 2000;
  };
  Xref xref;
};
MAKE_REFLECT_STRUCT(Config::CodeLens, localVariables);
MAKE_REFLECT_STRUCT(Config::Completion,
                    enableSnippets,
                    detailedLabel,
                    dropOldRequests,
                    filterAndSort,
                    includeMaxPathSize,
                    includeSuffixWhitelist,
                    includeBlacklist,
                    includeWhitelist);
MAKE_REFLECT_STRUCT(Config::Formatting, enabled)
MAKE_REFLECT_STRUCT(Config::Diagnostics,
                    blacklist,
                    whitelist,
                    frequencyMs,
                    onParse,
                    onType)
MAKE_REFLECT_STRUCT(Config::Highlight, enabled, blacklist, whitelist)
MAKE_REFLECT_STRUCT(Config::Index,
                    attributeMakeCallsToCtor,
                    blacklist,
                    whitelist,
                    comments,
                    enabled,
                    logSkippedPaths,
                    threads);
MAKE_REFLECT_STRUCT(Config::WorkspaceSymbol, maxNum, sort);
MAKE_REFLECT_STRUCT(Config::Xref, maxNum);
MAKE_REFLECT_STRUCT(Config,
                    compilationDatabaseCommand,
                    compilationDatabaseDirectory,
                    cacheDirectory,
                    cacheFormat,
                    resourceDirectory,

                    discoverSystemIncludes,
                    extraClangArguments,

                    progressReportFrequencyMs,
                    emitQueryDbBlocked,
                    emitInactiveRegions,

                    showDocumentLinksOnIncludes,

                    codeLens,
                    completion,
                    formatting,
                    diagnostics,
                    highlight,
                    index,
                    workspaceSymbol,
                    xref,

                    enableIndexOnDidChange);

extern Config* g_config;
