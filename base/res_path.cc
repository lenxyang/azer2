#include "azer/base/res_path.h"

#include "base/logging.h"
#include "base/files/file_util.h"
#include "base/strings/string_split.h"
#include "base/strings/string_tokenizer.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/base/res_path_parser.h"

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

const StringType ResPath::fullpath() const {
  return fullpath_;
}

Slice ResPath::filepath() const {
  return Slice(fullpath_.c_str() + filepath_slice_.begin, filepath_slice_.len);
}

Slice ResPath::proto() const {
  return Slice(fullpath_.c_str() + proto_slice_.begin, proto_slice_.len);
}

Slice ResPath::component() const {
  return Slice(fullpath_.c_str() + component_slice_.begin, component_slice_.len);
}

void ResPath::Init(const StringType& proto, const StringType& path, 
                   const StringType& component, PathType path_type) {
  fullpath_.clear();
  fullpath_.append(proto);
  proto_slice_.begin = 0;
  proto_slice_.len = proto.length();

  int filepath_index = fullpath_.length();
  fullpath_.append(path);
  filepath_slice_.begin = filepath_index;
  filepath_slice_.len = path.length();
  
  int component_index = fullpath_.length();
  fullpath_.append(component);
  component_slice_.begin = component_index;
  component_slice_.len = component.length();
  type_ = path_type;
}

void ResPath::clear() {
  filepath_slice_ = SliceIndex();
  proto_slice_ = SliceIndex();
  component_slice_ = SliceIndex();
  type_ = kRelativePath;
}

ResPath& ResPath::operator = (const ResPath& path) {
  fullpath_ = path.fullpath();
  filepath_slice_ = path.filepath_slice_;
  proto_slice_ = path.proto_slice_;
  component_slice_ = path.component_slice_;
  type_ = path.type();
  return *this;
}

void ResPath::OnPathChanged(const StringType& fullpath) {
  files::ResPathNormalizer normalizer(fullpath);
  normalizer.Normalize(this);
}

bool ResPath::AppendCopy(const ResPath& path, ResPath* output) const {
  CHECK(!path.empty());
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
  std::size_t pos = filepath().rfind(kSeperator);
  ResPath parent;
  Slice proto = this->proto();
  if (type() == kProtoPath && proto.length() == fullpath_.length()) {
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

  StringType component = FILE_PATH_LITERAL("");
  StringType file_path;
  if (pos == 1 && type_ == kAbsolutePath) {
    file_path = filepath().substr(0, pos + 1).as_string();
  } else {
    file_path = filepath().substr(0, pos).as_string();
  }
  parent.Init(proto.as_string(), file_path, component, type());
  return parent;
}

void ResPath::Normalize() {
}

Slice ResPath::DirName() const {
  Slice filepath = this->filepath();
  Slice base = BaseName();
  return Slice(filepath.data(), base.data() - filepath.data());
}

Slice ResPath::BaseName() const {
  Slice slice = fullpath_;
  int32 component_pos = fullpath_.find(kComponentSeperatorStr);
  if (component_pos != -1) {
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
  Slice component = this->component();
  if (!component.empty()) {
    DCHECK(component.data()[0] == FILE_PATH_LITERAL(':'));
    return component.substr(1);
  } else {
    Slice name = BaseName();
    uint32 pos = name.rfind(FILE_PATH_LITERAL("."));
    if (pos != StringType::npos)
      name = name.substr(0, pos);
    return name;
  }
}

std::vector<Slice> ResPath::dirs() const {
  std::vector<Slice> components;
  int cur = 0;
  Slice filepath = this->filepath();
  if (IsAbsolutePath()) {
    components.push_back(Slice(filepath.data(), 2));
    cur += 2;
  }

  while (cur >= 0 && cur < filepath.length()) {
    int prev = cur;
    cur = filepath.find(kSeperator, prev);
    if (cur > 0) {
      components.push_back(Slice(filepath.data() + prev, cur - prev));
      cur += 1;
    }
  }

  return components;
}

bool operator < (const ResPath& path1, const ResPath& path2) {
  return path1.fullpath() < path2.fullpath();
}
}  // namespace azer

