#include "azer/files/file_path.h"

#include "base/logging.h"
#include "base/files/file_util.h"
#include "base/strings/utf_string_conversions.h"

namespace azer {
namespace files {
const CharType FilePath::kSeperator = FILE_PATH_LITERAL('/');

const StringType FilePath::kRootPath = FILE_PATH_LITERAL("//");

FilePath::FilePath(const char* path) {
  ConvertPath<StringType>(path, &path_);
}

FilePath FilePath::AppendCopy(const StringType& path) const {
  FilePath new_path(path_);
  new_path.Append(path);
  return new_path;
}

void FilePath::Append(const StringType& str) {
  if (path_.back() == kSeperator) {
    path_.append(str);
  } else {
    path_.push_back(kSeperator);
    path_.append(str);
  }
}

bool SplitPackage(const StringType& full, StringType* path,
                  StringType* package) {
  size_t pos = full.find_last_of(':');
  if (pos < 0) return false;
  path->assign(full.substr(0, pos));
  package->assign(full.substr(pos + 1));
  return true;
}
}  // namespace files
}  // namespace azer

