#include "azer/files/res_path.h"

#include "base/logging.h"
#include "base/files/file_util.h"
#include "base/strings/string_util.h"
#include "base/strings/string_split.h"
#include "base/strings/utf_string_conversions.h"

namespace azer {

const CharType ResPath::kSeperator = FILE_PATH_LITERAL('/');
const StringType ResPath::kSeperatorStr = FILE_PATH_LITERAL("/");
const CharType ResPath::kComponentSeperator = FILE_PATH_LITERAL(':');
const StringType ResPath::kComponentSeperatorStr = FILE_PATH_LITERAL(":");
const StringType ResPath::kRootPath = FILE_PATH_LITERAL("//");

ResPath::ResPath(const char* path) {
  ConvertPath<StringType>(path, &fullpath_);
  OnFullpathChanged(this->fullpath());
}

ResPath::ResPath(const StringType& fullpath)
    : fullpath_(fullpath) {
  OnFullpathChanged(this->fullpath());
}

ResPath::ResPath(const ResPath& fullpath)
    : fullpath_(fullpath.value()){
  OnFullpathChanged(this->fullpath());
}

ResPath::ResPath(const StringType& path, const StringType& component) {
  StringType str = path;
  CHECK(!EndsWith(str, kSeperatorStr, true));
  str.append(kComponentSeperatorStr);
  str.append(component);
  OnFullpathChanged(this->fullpath());
}

ResPath::ResPath(const ResPath& path, const StringType& component) {
  StringType str = path.value();
  CHECK(!EndsWith(str, kSeperatorStr, true));
  str.append(kComponentSeperatorStr);
  str.append(component);
  OnFullpathChanged(this->fullpath());
}

void ResPath::OnFullpathChanged(const StringType& fullpath) {
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

ResPath ResPath::AppendCopy(const StringType& path) const {
  ResPath new_path(path_);
  new_path.Append(path);
  return new_path;
}

void ResPath::Append(const StringType& str) {
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

