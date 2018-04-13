#include "method.h"

MethodType kMethodType_Unknown = "$unknown";
MethodType kMethodType_Exit = "exit";
MethodType kMethodType_TextDocumentPublishDiagnostics =
    "textDocument/publishDiagnostics";
MethodType kMethodType_CqueryPublishInactiveRegions =
    "$cquery/publishInactiveRegions";
MethodType kMethodType_CqueryPublishSemanticHighlighting =
    "$cquery/publishSemanticHighlighting";

std::string ToString(const lsRequestId& id) {
  if (std::holds_alternative<std::string>(id)) {
    return std::get<std::string>(id);
  } else if (std::holds_alternative<int64_t>(id)) {
    return std::to_string(std::get<int64_t>(id));
  } else {
    return "";
  }
}

InMessage::~InMessage() = default;

lsRequestId RequestInMessage::GetRequestId() const {
  return id;
}

lsRequestId NotificationInMessage::GetRequestId() const {
  return std::monostate();
}