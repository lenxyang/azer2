#include "azer/files/res_path.h"

#include "base/logging.h"
#include "base/files/file_util.h"
#include "base/strings/string_split.h"
#include "base/strings/string_tokenizer.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/files/res_path_normalizer.h"

namespace azer {

const CharType ResPath::kSeperator = FILE_PATH_LITERAL('/');
const StringType ResPath::kSeperatorStr = FILE_PATH_LITERAL("/");
const CharType ResPath::kComponentSeperator = FILE_PATH_LITERAL(':');
const StringType ResPath::kComponentSeperatorStr = FILE_PATH_LITERAL(":");
const StringType ResPath::kRootPath = FILE_PATH_LITERAL("//");

bool ResPath::ValidPath(const StringType& str) {
  return true;
}

ResPath::PathType ResPath::CalcPathType(const StringType& str) {
  if (!ValidPath(str)) {
    return kInvalidPath;
  }

  return kAbsolutePath;
}

ResPath::ResPath() 
    : type_(kRelativePath) {
}

ResPath::ResPath(const CharType* path) {
  OnPathChanged(StringType(path));
}

ResPath::ResPath(const StringType& rawpath) {
  OnPathChanged(rawpath);
}

ResPath::ResPath(const ResPath& path) {
  *this = path;
}

ResPath::ResPath(const StringType& path, const StringType& component) {
  StringType str = path;
  str.append(kComponentSeperatorStr);
  str.append(component);
  OnPathChanged(str);
}

ResPath::ResPath(const ResPath& path, const StringType& component) {
  StringType str = path.fullpath();
  str.append(kComponentSeperatorStr);
  str.append(component);
  OnPathChanged(str);
}

void ResPath::Init(const StringType& proto, const StringType& path, 
                   const StringType& component, PathType path_type) {
  fullpath_.clear();
  fullpath_.append(proto);
  proto_ = Slice(fullpath_);

  int file_path_index = fullpath_.length();
  fullpath_.append(path);
  file_path_ = Slice(fullpath_.c_str() + file_path_index, path.length());
  
  int component_index = fullpath_.length();
  fullpath_.append(component);
  component_ = Slice(fullpath_.c_str() + component_index, component.length());
  type_ = path_type;
}

void ResPath::clear() {
  fullpath_ = AZER_LITERAL("");
  file_path_ = AZER_LITERAL("");
  component_ = AZER_LITERAL("");
  type_ = kRelativePath;
}

ResPath& ResPath::operator = (const ResPath& path) {
  fullpath_ = path.fullpath();
  file_path_ = path.filepath();
  proto_ = path.proto();
  component_ = path.component();
  type_ = path.type();
  return *this;
}

void ResPath::OnPathChanged(const StringType& fullpath) {
  ResPathNormalizer normalizer(fullpath);
  normalizer.Normalize(this);
}

bool ResPath::AppendCopy(const ResPath& path, ResPath* output) const {
  ResPath new_path(*this);
  if (new_path.Append(path)) {
    *output = new_path;
    return type() != kInvalidPath;
  } else {
    return false;
  }
}

ResPath ResPath::AppendCopyOrDie(const ResPath& path) const {
  ResPath new_path(*this);
  CHECK(new_path.Append(path));
  return new_path;
}

bool ResPath::Append(const ResPath& str) {
  StringType pathstr = this->fullpath();
  pathstr.append(str.fullpath());
  OnPathChanged(pathstr);
  return type() != kInvalidPath;
}

ResPath ResPath::parent() const {
  std::size_t pos = file_path_.rfind(kSeperator);
  ResPath parent;
  if (type() == kProtoPath && proto_.length() == fullpath_.length()) {
    parent.type_ = kInvalidPath;
    return parent;
  } else if (type() == kAbsolutePath && fullpath_.length() == 2u) {
    parent.type_ = kInvalidPath;
    return parent;
  } else if (type() == kInvalidPath) {
    parent.type_ = kInvalidPath;
    return parent;
  } else if (type() == kRelativePath && pos == StringType::npos) {
    parent.type_ = kInvalidPath;
    return parent;
  }

  StringType component_ = FILE_PATH_LITERAL("");
  StringType file_path;
  if (pos == 1 && type_ == kAbsolutePath) {
    file_path = filepath().substr(0, pos + 1).as_string();
  } else {
    file_path = filepath().substr(0, pos).as_string();
  }
  parent.Init(proto_.as_string(), file_path, component().as_string(), type());
  return parent;
}

void ResPath::Normalize() {
}

Slice ResPath::filename() const {
  Slice slice;
  int32 component_pos = fullpath_.find(kComponentSeperatorStr);
  if (component_pos == -1) {
    slice = Slice(fullpath_.c_str(), component_pos);
  }
  
  int32 last_dir = slice.find_last_of(kSeperatorStr);
  if (last_dir == static_cast<int32>(slice.length()) - 1) {
    return Slice();
  } else if (last_dir != -1) {
    return slice.substr(last_dir + 1);
  } else {
    return slice;
  }
}

Slice ResPath::component_name() const {
  if (!component_.empty()) {
    DCHECK(component_.data()[0] == FILE_PATH_LITERAL(':'));
    return component_.substr(1);
  } else {
    Slice name = filename();
    uint32 pos = name.rfind(FILE_PATH_LITERAL("."));
    if (pos != StringType::npos)
      name = name.substr(0, pos);
    return name;
  }
}

std::vector<Slice> ResPath::dirs() const {
  std::vector<Slice> components, vec;
  int cur = 0;
  if (IsAbsolutePath()) {
    components.push_back(Slice(filepath().data(), 2));
    cur += 2;
  }

  while (cur > 0) {
    int prev = cur;
    cur = fullpath().find(cur, kSeperator);
    if (cur > 0) {
      components.push_back(Slice(filepath().data() + prev, cur - prev));
    }
  }

  return components;
}

bool operator < (const ResPath& path1, const ResPath& path2) {
  return path1.fullpath() < path2.fullpath();
}
}  // namespace azer

