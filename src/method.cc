#include "method.h"

#include <doctest/doctest.h>
#include <iostream>
#include "serializers/json.h"

MethodType kMethodType_Unknown = "$unknown";
MethodType kMethodType_Exit = "exit";
MethodType kMethodType_TextDocumentPublishDiagnostics =
    "textDocument/publishDiagnostics";
MethodType kMethodType_CqueryPublishInactiveRegions =
    "$cquery/publishInactiveRegions";
MethodType kMethodType_CqueryQueryDbStatus = "$cquery/queryDbStatus";
MethodType kMethodType_CqueryPublishSemanticHighlighting =
    "$cquery/publishSemanticHighlighting";

void Reflect(Reader& visitor, lsRequestId& value) {
  if (visitor.IsInt()) {
    value.type = lsRequestId::kInt;
    value.value = visitor.GetInt();
  } else if (visitor.IsInt64()) {
    value.type = lsRequestId::kInt;
    // `lsRequestId.value` is an `int`, so we're forced to truncate.
    value.value = static_cast<int>(visitor.GetInt64());
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
      std::string str = std::to_string(value.value);
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

TEST_SUITE("lsRequestId") {
  TEST_CASE("To string") {
    // FIXME: Add test reflection helpers; make it easier to use.
    rapidjson::StringBuffer output;
    rapidjson::Writer<rapidjson::StringBuffer> writer(output);
    JsonWriter json_writer(&writer);

    json_writer.StartArray(0);

    lsRequestId id;
    id.value = 3;

    id.type = lsRequestId::kNone;
    Reflect(json_writer, id);

    id.type = lsRequestId::kInt;
    Reflect(json_writer, id);

    id.type = lsRequestId::kString;
    Reflect(json_writer, id);

    json_writer.EndArray();

    REQUIRE(std::string(output.GetString()) == "[null,3,\"3\"]");
  }
}
