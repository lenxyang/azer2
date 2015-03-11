#include "azer/files/file_path.h"

#include "base/logging.h"
#include "base/files/file_util.h"
#include "base/strings/string_util.h"
#include "base/strings/string_split.h"
#include "base/strings/utf_string_conversions.h"

namespace azer {

const CharType FilePath::kSeperator = FILE_PATH_LITERAL('/');
const StringType FilePath::kSeperatorStr = FILE_PATH_LITERAL("/");
const CharType FilePath::kComponentSeperator = FILE_PATH_LITERAL(':');
const StringType FilePath::kComponentSeperatorStr = FILE_PATH_LITERAL(":");
const StringType FilePath::kRootPath = FILE_PATH_LITERAL("//");

FilePath::FilePath(const char* path) {
  ConvertPath<StringType>(path, &fullpath_);
  OnFullpathChanged(this->fullpath());
}

FilePath::FilePath(const StringType& fullpath)
    : fullpath_(fullpath) {
  OnFullpathChanged(this->fullpath());
}

FilePath::FilePath(const FilePath& fullpath)
    : fullpath_(fullpath.value()){
  OnFullpathChanged(this->fullpath());
}

FilePath::FilePath(const StringType& path, const StringType& component) {
  StringType str = path;
  CHECK(!EndsWith(str, kSeperatorStr, true));
  str.append(kComponentSeperatorStr);
  str.append(component);
  OnFullpathChanged(this->fullpath());
}

FilePath::FilePath(const FilePath& path, const StringType& component) {
  StringType str = path.value();
  CHECK(!EndsWith(str, kSeperatorStr, true));
  str.append(kComponentSeperatorStr);
  str.append(component);
  OnFullpathChanged(this->fullpath());
}

void FilePath::OnFullpathChanged(const StringType& fullpath) {
  std::vector<StringType> vec;
  ::base::SplitString(fullpath, kComponentSeperator, &vec);
  if (vec.size() > 0u) {
    path_ = vec[0];
  }
  if (vec.size() > 1u) {
    component_ = vec[1];
  }

  CHECK_LE(vec.size(), 2u);
}

FilePath FilePath::AppendCopy(const StringType& path) const {
  FilePath new_path(path_);
  new_path.Append(path);
  return new_path;
}

void FilePath::Append(const StringType& str) {
  if (fullpath_.back() == kSeperator) {
    fullpath_.append(str);
  } else {
    fullpath_.push_back(kSeperator);
    fullpath_.append(str);
  }

  OnFullpathChanged(this->fullpath());
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

