#include "azer/effectgen/exception.h"

#include <ostream>
#include "base/logging.h"

namespace azer {
namespace effectgen {

std::ostream& operator << (const ExceptLoc& loc, std::ostream& os) {
  os << loc.file << ":" << loc.lineno;
  return os;
}

Exception::Exception(ExceptLoc location, const ::base::debug::StackTrace& trace)
    : location_(location),
      trace_(trace) {
}

// class LexerException
}  // namespace effectgen
}  // namespace azer
