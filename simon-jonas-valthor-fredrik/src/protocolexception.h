#include <string>

#ifndef PROTOCOL_EXCEPTION_H
#define PROTOCOL_EXCEPTION_H

struct ProtocolException {
  int errType;
  std::string msg;
};

#endif
