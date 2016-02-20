#pragma once

#include "base/files/file_path.h"
#include "azer/base/export.h"
#include "azer/base/string.h"

namespace azer {
namespace files {
class ResPathNormalizer;
}

// ResPath's format 
// '//.../.../../filename:component
// ResPath's root is setting in FileSystem
// for example, if the filesystem root is c:/mydir,
// the azer::FileSystem "//a", for native files is c:/mydir/a
class AZER_EXPORT ResPath {
 public:
  enum PathType {
    kRelativePath,
    kAbsolutePath,
    kProtoPath,
    kInvalidPath,
  };

  ResPath();
  explicit ResPath(const CharType* path);
  explicit ResPath(const StringType& path);
  ResPath(const ResPath& path);
  ResPath(const StringType& path, const StringType& component);
  ResPath(const ResPath& path, const StringType& component);

  ResPath& operator = (const ResPath& path);

  ResPath AppendCopyOrDie(const ResPath& path) const;
  bool AppendCopy(const ResPath& path, ResPath* output) const;
  bool Append(const ResPath& str);
  void Normalize();
  bool empty() const { return fullpath_.empty();}
  void clear();

  std::vector<Slice> dirs() const;
  
  // full path (raw path with normalized)
  const StringType fullpath() const;

  // file path (full path without proto header)
  Slice filepath() const;

  // file name: the last part of dirs
  Slice BaseName() const;
  Slice DirName() const;

  // proto: the proto header, like "http://"
  Slice proto() const;

  // component_: component party
  // for example: net://www.sina.com/a:bcom, :bcom is the component party
  Slice component() const;

  // component name
  // if has component party, the name is component part without comma
  // if has no component part, the name will be same as file name with out suppfix
  Slice component_name() const;

  ResPath parent() const;
  PathType type() const { return type_;}
  bool has_proto() const { return proto_slice_.len > 0;}
  bool has_component() const { return component_slice_.len > 0;}
  bool IsRoot() const { return fullpath_ == kRootPath;}
  bool IsAbsolutePath() const { return type_ == kAbsolutePath;}
  bool IsRelativePath() const { return type_ == kRelativePath;}

  friend AZER_EXPORT bool operator < (const ResPath& path1, const ResPath& path2);

  static PathType CalcPathType(const StringType& str);
  static bool ValidPath(const StringType& str);
  static const StringType kRootPath;
  static const StringType kSeperatorStr;
  static const CharType kSeperator;
  static const StringType kComponentSeperatorStr;
  static const CharType kComponentSeperator;
 protected:
  void Init(const StringType& proto, const StringType& path, 
            const StringType& component, PathType pathtype);
  void OnPathChanged(const StringType& type);

  struct SliceIndex {
    int begin;
    int len;
    SliceIndex() : begin(0), len(0) {}
  };
  StringType fullpath_;
  SliceIndex filepath_slice_;
  SliceIndex proto_slice_;
  SliceIndex component_slice_;
  std::vector<Slice> dirs_;
  PathType type_;

  friend class files::ResPathNormalizer;
};
}  // namespace azer
