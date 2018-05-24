#pragma once

#include "method.h"
#include "query.h"
#include "threaded_queue.h"

#include <memory>

struct ICacheManager;
struct lsBaseOutMessage;

struct Stdout_Request {
  MethodType method;
  std::string content;
};

struct Index_Request {
  AbsolutePath path;
  // TODO: make |args| a string that is parsed lazily.
  std::vector<std::string> args;
  bool is_interactive;
  std::string contents;  // Preloaded contents.
  std::shared_ptr<ICacheManager> cache_manager;
  lsRequestId id;

  Index_Request(const AbsolutePath& path,
                const std::vector<std::string>& args,
                bool is_interactive,
                const std::string& contents,
                const std::shared_ptr<ICacheManager>& cache_manager,
                lsRequestId id = {});
};

struct Index_DoIdMap {
  std::unique_ptr<IndexFile> current;
  std::unique_ptr<IndexFile> previous;
  std::shared_ptr<ICacheManager> cache_manager;

  bool is_interactive = false;
  bool write_to_disk = false;
  bool load_previous = false;

  Index_DoIdMap(std::unique_ptr<IndexFile> current,
                const std::shared_ptr<ICacheManager>& cache_manager,
                bool is_interactive,
                bool write_to_disk);
};

struct Index_OnIdMapped {
  struct File {
    std::unique_ptr<IndexFile> file;
    std::unique_ptr<IdMap> ids;

    File(std::unique_ptr<IndexFile> file, std::unique_ptr<IdMap> ids);
  };

  std::unique_ptr<File> previous;
  std::unique_ptr<File> current;
  std::shared_ptr<ICacheManager> cache_manager;

  bool is_interactive;
  bool write_to_disk;

  Index_OnIdMapped(const std::shared_ptr<ICacheManager>& cache_manager,
                   bool is_interactive,
                   bool write_to_disk);
};

struct Index_OnIndexed {
  IndexUpdate update;

  Index_OnIndexed(IndexUpdate&& update);
};

class QueueManager {
 public:
  static QueueManager* instance() { return instance_.get(); }
  static void Init();
  static void WriteStdout(MethodType method, lsBaseOutMessage& response);

  bool HasWork();

  std::shared_ptr<MultiQueueWaiter> querydb_waiter;
  std::shared_ptr<MultiQueueWaiter> indexer_waiter;
  std::shared_ptr<MultiQueueWaiter> stdout_waiter;

  // Messages received by "stdout" thread.
  ThreadedQueue<Stdout_Request> for_stdout;

  // Runs on querydb thread.
  ThreadedQueue<std::unique_ptr<InMessage>> for_querydb;
  ThreadedQueue<Index_DoIdMap> do_id_map;

  // Runs on indexer threads.
  ThreadedQueue<Index_Request> index_request;
  ThreadedQueue<Index_DoIdMap> load_previous_index;
  ThreadedQueue<Index_OnIdMapped> on_id_mapped;

  // Index_OnIndexed is split into two queues. on_indexed_for_querydb is
  // limited to a mediumish length and is handled only by querydb. When that
  // list grows too big, messages are added to on_indexed_for_merge which will
  // be processed by the indexer.
  ThreadedQueue<Index_OnIndexed> on_indexed_for_merge;
  ThreadedQueue<Index_OnIndexed> on_indexed_for_querydb;

 private:
  explicit QueueManager();
  static std::unique_ptr<QueueManager> instance_;
};
