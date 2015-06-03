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

ResPath::ResPath(const CharType* path)
    : rawpath_(StringType(path)) {
  OnPathChanged(this->rawpath());
}

ResPath::ResPath(const StringType& rawpath)
    : rawpath_(rawpath) {
  OnPathChanged(this->rawpath());
}

ResPath::ResPath(const ResPath& path) {
  *this = path;
}

ResPath::ResPath(const StringType& path, const StringType& component) {
  StringType str = path;
  str.append(kComponentSeperatorStr);
  str.append(component);
  OnPathChanged(this->rawpath());
}

ResPath::ResPath(const ResPath& path, const StringType& component) {
  StringType str = path.rawpath();
  str.append(kComponentSeperatorStr);
  str.append(component);
  OnPathChanged(this->rawpath());
}

void ResPath::clear() {
  rawpath_ = AZER_LITERAL("");
  fullpath_ = AZER_LITERAL("");
  file_path_ = AZER_LITERAL("");
  component_ = AZER_LITERAL("");
  type_ = kRelativePath;
}

ResPath& ResPath::operator = (const ResPath& path) {
  rawpath_ = path.rawpath();
  fullpath_ = path.fullpath();
  file_path_ = path.filepath();
  proto_ = path.proto();
  component_ = path.component();
  type_ = path.type();
  return *this;
}

void ResPath::OnPathChanged(const StringType& fullpath) {
  ResPathNormalizer normalizer;
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
  if (!rawpath_.empty()) {
    if (rawpath_.back() != kSeperator) {
      rawpath_.push_back(kSeperator);
    }
  } else {
    rawpath_ = str.rawpath();
  }
  
  rawpath_.append(str.fullpath());
  OnPathChanged(this->fullpath());
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

  parent.type_ = type_;
  parent.component_ = FILE_PATH_LITERAL("");
  if (pos == 1 && type_ == kAbsolutePath) {
    parent.file_path_ = filepath().substr(0, pos + 1);
  } else {
    parent.file_path_ = filepath().substr(0, pos);
  }
  parent.fullpath_.append(proto_);
  parent.fullpath_.append(parent.file_path_);
  return parent;
}

void ResPath::Normalize() {
}

StringType ResPath::filename() const {
  StringType path;
  int32 component_pos = fullpath_.find(kComponentSeperatorStr);
  if (component_pos == -1) {
    path = fullpath_.substr(0, component_pos);
  }
  
  int32 last_dir = path.find_last_of(kSeperatorStr);
  if (last_dir == static_cast<int32>(path.length()) - 1) {
    return StringType(FILE_PATH_LITERAL(""));
  } else if (last_dir != -1) {
    return path.substr(last_dir + 1);
  } else {
    return path;
  }
}

StringType ResPath::component_name() const {
  if (!component_.empty()) {
    DCHECK(StartsWith(component_, FILE_PATH_LITERAL(":"), true));
    return component_.substr(1);
  } else {
    StringType name = std::move(filename());
    uint32 pos = name.rfind(FILE_PATH_LITERAL("."));
    if (pos != StringType::npos)
      name = name.substr(0, pos);
    return name;
  }
}

std::vector<StringType> ResPath::dirs() const {
  std::vector<StringType> components, vec;
  if (IsAbsolutePath()) 
    components.push_back(kRootPath);

  ::base::SplitString(parent().fullpath(), kSeperator, &vec);
  for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
    if (*iter == kSeperatorStr || iter->empty())
      continue;
    components.push_back(*iter);
  }

  return components;
}

bool operator < (const ResPath& path1, const ResPath& path2) {
  return path1.fullpath() < path2.fullpath();
}
}  // namespace azer

