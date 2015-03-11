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
  ResPath(const char* path);
  ResPath(const StringType& fullpath);
  ResPath(const ResPath& fullpath);
  ResPath(const StringType& path, const StringType& component);
  ResPath(const ResPath& path, const StringType& component);

  ResPath& operator = (const ResPath& path) {
    path_ = path.value();
    return *this;
  }

  const StringType& value() const { return path_;}

  ResPath AppendCopy(const StringType& path) const;
  void Append(const StringType& str);
  bool empty() const { return path_.empty();}

  const StringType& fullpath() const { return fullpath_;}
  const StringType& filepath() const { return path_;}
  const StringType& component() const { return component_;}

  ResPath parent() const;

  static const StringType kRootPath;
  static const StringType kSeperatorStr;
  static const CharType kSeperator;
  static const StringType kComponentSeperatorStr;
  static const CharType kComponentSeperator;
 protected:
  void OnFullpathChanged(const StringType& type);
  StringType fullpath_;
  StringType path_;
  StringType component_;
  
};

AZER_EXPORT bool SplitPackage(const StringType& full, StringType* path,
                              StringType* package);
}  // namespace azer
