#include "azer/base/build_info.h"

#include "base/files/file_path.h"
#include "base/strings/utf_string_conversions.h"
#include "base/logging.h"

namespace azer {
extern const int kBuildRootDirLength;
extern const ::base::FilePath::StringType kBuildRootDir;
extern const char* kBuildTime;
extern const char* kBuildMachine;

bool CheckInBuildDir(const ::base::FilePath& path) {
  return ::base::FilePath(kBuildRootDir).IsParent(path);
}


::base::FilePath GetRelativePath(const char* utf8path) {
#if defined(OS_WIN)
  return GetRelativePath(::base::UTF8ToWide(utf8path));
#else
  return GetRelativePath(::base::FilePath(utf8path));
#endif
}

::base::FilePath GetRelativePath(const ::base::FilePath::StringType& path) {
  return GetRelativePath(::base::FilePath(path));
}

::base::FilePath GetRelativePath(const ::base::FilePath& path) {
  DCHECK(CheckInBuildDir(path));
  return ::base::FilePath(path.value().substr(kBuildRootDir.length()));
}
}  // namespace azer

