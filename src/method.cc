#include "method.h"

MethodType kMethodType_Unknown = "$unknown";
MethodType kMethodType_Exit = "exit";
MethodType kMethodType_TextDocumentPublishDiagnostics =
    "textDocument/publishDiagnostics";
MethodType kMethodType_CqueryPublishInactiveRegions =
    "$cquery/publishInactiveRegions";
MethodType kMethodType_CqueryPublishSemanticHighlighting =
    "$cquery/publishSemanticHighlighting";

void Reflect(Reader& visitor, lsRequestId& value) {
  if (visitor.IsInt()) {
    value.type = lsRequestId::kInt;
    value.value = visitor.GetInt();
  } else if (visitor.IsInt64()) {
    value.type = lsRequestId::kInt;
    value.value = visitor.GetInt64();
  } else if (visitor.IsString()) {
    value.type = lsRequestId::kString;
    std::string s = visitor.GetString();
    value.value = atoi(s.c_str());
  } else {
    value.type = lsRequestId::kNone;
    value.value = -1;
  }
}

void Reflect(Writer& visitor, lsRequestId& value) {
  switch (value.type) {
    case lsRequestId::kNone:
      visitor.Null();
      break;
    case lsRequestId::kInt:
      visitor.Int(value.value);
      break;
    case lsRequestId::kString:
      std::string str;
      visitor.String(str.c_str(), str.length());
      break;
  }
}

std::string ToString(const lsRequestId& id) {
  if (id.type != lsRequestId::kNone)
    return std::to_string(id.value);
  return "";
}

InMessage::~InMessage() = default;

lsRequestId RequestInMessage::GetRequestId() const {
  return id;
}

lsRequestId NotificationInMessage::GetRequestId() const {
  return lsRequestId();
}