#include "azer/files/res_path.h"

#include "base/logging.h"
#include "base/files/file_util.h"
#include "base/strings/string_split.h"
#include "base/strings/string_tokenizer.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"

namespace azer {

const CharType ResPath::kSeperator = FILE_PATH_LITERAL('/');
const StringType ResPath::kSeperatorStr = FILE_PATH_LITERAL("/");
const CharType ResPath::kComponentSeperator = FILE_PATH_LITERAL(':');
const StringType ResPath::kComponentSeperatorStr = FILE_PATH_LITERAL(":");
const StringType ResPath::kRootPath = FILE_PATH_LITERAL("//");

bool ResPath::ValidPath(const StringType& str) {
  
}

PathType ResPath::CalcPathType(StringType str) {
  if (!ValidPath(str)) {
    return InvalidPath;
  }

  if (StartsWith(str, kRootPath, true)) {
    return kAbsolutePath;
  } else {
    std::size_t proto_pos = str.find(FILE_PATH_LITERAL("://"));
  }
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

ResPath& ResPath::operator = (const ResPath& path) {
  fullpath_ = path->fullpath();
  OnPathChanged(this->fullpath());
  return *this;
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

bool ResPath::AppendCopy(const ResPath& path, ResPath* output) const {
  ResPath new_path(*this);
  if (new_path.Append(path)) {
    *output = new_path;
    return true;
  } else {
    return false;
  }
}

ResPath ResPath::AppendCopyOrDie(const Respath& path) const {
  ResPath new_path(*this);
  CHECK(new_path.Append(path));
  return new_path;
}

bool ResPath::Append(const ResPath& str) {
  if (has_component()) { return false;}
  if (str.type() != kRelativePath) { return false;}

  if (fullpath_.back() != kSeperator) {
    fullpath_.push_back(kSeperator);
  }

  fullpath_.append(str);
  OnPathChanged(this->fullpath());
  return true;
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

