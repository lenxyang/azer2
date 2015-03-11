#include "azer/files/test/test_util.h"

#include "azer/base/strings.h"

namespace azer {
namespace test {
ResPath Utf8ResPath(const char* str) {
  StringType pathstr;
  ConvertPath(str, &pathstr);
  return ResPath(pathstr);
}

ResPath Utf8ResPath(const std::string& str) {
  StringType pathstr;
  ConvertPath(str, &pathstr);
  return ResPath(pathstr);
}
}  // namespace test
}  // namespace azer
