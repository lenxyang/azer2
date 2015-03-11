#pragma once

#include "azer/files/res_path.h"

namespace azer {
namespace test {
ResPath Utf8ResPath(const char* str);
ResPath Utf8ResPath(const std::string& str);
}  // namespace test
}  // namespace azer
