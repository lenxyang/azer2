#pragma once

#include <ostream>

#include "base/debug/stack_trace.h"
#include "base/strings/stringprintf.h"

namespace azer {
namespace effectgen {

struct ExceptLoc {
  std::string file;
  int lineno;
  int offset;
};

std::ostream& operator << (const ExceptLoc& loc, std::ostream& os);

class Exception {
 public:
  Exception(ExceptLoc location, const ::base::debug::StackTrace& trace);
  virtual const std::string message() const = 0;

  const ExceptLoc& location() { return location_;}
 protected:
  ExceptLoc location_;
  ::base::debug::StackTrace trace_;
};

class LexerException : public Exception {
 public:
  LexerException(const std::string& message, ExceptLoc location,
                 const ::base::debug::StackTrace& trace)
      : Exception(location, trace) {
  }
  const std::string message() const override { return message_;}
 protected:
  std::string message_;
};

class InvalidNumException : public LexerException {
 public:
  using StackTrace = ::base::debug::StackTrace;
  InvalidNumException(const std::string& message, 
                      ExceptLoc location, 
                      const StackTrace& trace = StackTrace())
      : LexerException(message, location, trace) {
  }
};
}  // namespace effectgen
}  // namespace azer
