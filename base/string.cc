#include "azer/base/string.h"

#include <string>
#include <ostream>

#include "base/strings/utf_string_conversions.h"
#include "base/strings/string16.h"
#include "base/logging.h"

namespace azer {

std::ostream& operator << (std::ostream& os, const ::base::FilePath& path) {
  os << ::base::string16(path.value());
  return os;
}
}  // namespace azer
