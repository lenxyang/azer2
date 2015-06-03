#pragma once

#include "base/files/file_path.h"
#include "azer/base/export.h"
#include "azer/base/string.h"

namespace azer {
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

  std::vector<StringType> dirs() const;
  // raw path ( which has no normalized )
  const StringType& rawpath() const { return rawpath_;}

  // full path (raw path with normalized)
  const StringType& fullpath() const { return fullpath_;}

  // file path (full path without proto header)
  const StringType& filepath() const { return file_path_;}

  // file name: the last part of dirs
  StringType filename() const;

  // proto: the proto header, like "http://"
  const StringType& proto() const { return proto_;}

  // component_: component party
  // for example: net://www.sina.com/a:bcom, :bcom is the component party
  const StringType& component() const { return component_;}

  // component name
  // if has component party, the name is component part without comma
  // if has no component part, the name will be same as file name with out suppfix
  StringType component_name() const;

  ResPath parent() const;
  PathType type() const { return type_;}
  bool has_proto() const { return !proto_.empty();}
  bool has_component() const { return !component_.empty();}
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
  void OnPathChanged(const StringType& type);
  StringType rawpath_;
  StringType fullpath_;
  StringType file_path_;
  StringType proto_;
  StringType component_;
  std::vector<StringType> dirs_;
  PathType type_;

  friend class ResPathNormalizer;
};
}  // namespace azer
