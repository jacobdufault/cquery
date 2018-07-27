#include "lsp_diagnostic.h"

#include "match.h"
#include "working_files.h"

struct DiagnosticsEngine {
  void Init();
  void Publish(WorkingFiles* working_files,
               std::string path,
               std::vector<lsDiagnostic> diagnostics);

  std::unique_ptr<GroupMatch> match_;
  int64_t nextPublish_ = 0;
  int frequencyMs_;
};
