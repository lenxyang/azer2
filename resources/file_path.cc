#include "azer/resources/file_path.h"

#include "base/logging.h"
#include "base/files/file_util.h"
#include "base/strings/utf_string_conversions.h"

namespace azer {
const CharType ResFilePath::kSeperator = FILE_PATH_LITERAL('/');

const StringType ResFilePath::kRootPath = FILE_PATH_LITERAL("//");

ResFilePath ResFilePath::AppendCopy(const StringType& path) const {
  ResFilePath new_path(path_);
  new_path.Append(path);
  return new_path;
}

void ResFilePath::Append(const StringType& str) {
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
}  // namespace azer
