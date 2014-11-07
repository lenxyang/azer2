#pragma once

#include "base/files/file_path.h"
#include "azer/base/string.h"

namespace azer {
bool CheckInBuildDir(const ::base::FilePath& path);
::base::FilePath GetRelativePath(const ::base::FilePath::StringType& path);
::base::FilePath GetRelativePath(const ::base::FilePath& path);
::base::FilePath GetRelativePath(const char* utf8path);
}  // namespace azer
