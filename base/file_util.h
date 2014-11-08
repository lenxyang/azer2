#pragma once

#include <string>

#include "azer/base/render_export.h"
#include "base/files/file_path.h"

namespace azer {
AZER_EXPORT bool ReadFileToString(const ::base::FilePath& path, std::string* content,
                                  int max_size = 0x8fffffff);
}  // namespace azer 
