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

PathType ResPath::CalcPathType(StringType str) {
}

ResPath::ResPath(const char* path) {
  ConvertPath<StringType>(path, &fullpath_);
  OnPathChanged(this->fullpath());
}

ResPath::ResPath(const StringType& fullpath)
    : fullpath_(fullpath) {
  OnPathChanged(this->fullpath());
}

ResPath::ResPath(const ResPath& fullpath)
    : fullpath_(fullpath.value()){
  OnPathChanged(this->fullpath());
}

ResPath::ResPath(const StringType& path, const StringType& component) {
  StringType str = path;
  CHECK(!EndsWith(str, kSeperatorStr, true));
  str.append(kComponentSeperatorStr);
  str.append(component);
  OnPathChanged(this->fullpath());
}

ResPath::ResPath(const ResPath& path, const StringType& component) {
  StringType str = path.value();
  CHECK(!EndsWith(str, kSeperatorStr, true));
  str.append(kComponentSeperatorStr);
  str.append(component);
  OnPathChanged(this->fullpath());
}

void ResPath::OnPathChanged(const StringType& fullpath) {
  std::vector<StringType> vec;
  ::base::SplitString(fullpath, kComponentSeperator, &vec);
  if (vec.size() > 0u) {
    path_ = vec[0];
  }
  if (vec.size() > 1u) {
    component_ = vec[1];
  }

  CHECK_LE(vec.size(), 2u);
  path_type_ = CalcPathType(fullpath);
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

  OnPathChanged(this->fullpath());
}

ResPath ResPath::parent() const {
  CHECK(!IsRoot());
  StringType pathstr = filepath().substr();
  std::size_t pos = pathstr.rfind(kSeperator);
  if (pos != StringType::npos) {
    return ResPath(filepath().substr(0, pathstr.length() - pos));
  } else {
    return ResPath("//");
  }
}

void ResPath::Normalize() {
}

}  // namespace azer

