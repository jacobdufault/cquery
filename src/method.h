#pragma once

#include "serializer.h"
#include "utils.h"

#include <string>

using MethodType = const char*;
extern MethodType kMethodType_Unknown;
extern MethodType kMethodType_Exit;
extern MethodType kMethodType_TextDocumentPublishDiagnostics;
extern MethodType kMethodType_CqueryPublishInactiveRegions;
extern MethodType kMethodType_CqueryQueryDbStatus;
extern MethodType kMethodType_CqueryPublishSemanticHighlighting;

struct lsRequestId {
  // The client can send the request id as an int or a string. We should output
  // the same format we received.
  enum Type { kNone, kInt, kString };
  Type type = kNone;

  int value = -1;

  bool has_value() const { return type != kNone; }
};
void Reflect(Reader& visitor, lsRequestId& value);
void Reflect(Writer& visitor, lsRequestId& value);

// Debug method to convert an id to a string.
std::string ToString(const lsRequestId& id);

struct InMessage {
  virtual ~InMessage();

  virtual MethodType GetMethodType() const = 0;
  virtual lsRequestId GetRequestId() const = 0;
};

struct RequestInMessage : public InMessage {
  // number or string, actually no null
  lsRequestId id;
  lsRequestId GetRequestId() const override;
};

// NotificationInMessage does not have |id|.
struct NotificationInMessage : public InMessage {
  lsRequestId GetRequestId() const override;
};
