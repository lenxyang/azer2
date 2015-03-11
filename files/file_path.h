#pragma once

#include "base/files/file_path.h"
#include "azer/base/export.h"
#include "azer/base/string.h"

namespace azer {
// FilePath's root is setting in FileSystem
// for example, if the filesystem root is c:/mydir,
// the azer::FileSystem "//a", for native files is c:/mydir/a
class AZER_EXPORT FilePath {
 public:
  FilePath(const char* path);
  FilePath(const StringType& path) : path_(path) {}
  FilePath(const FilePath& path) : path_(path.value()) {}

  FilePath& operator = (const FilePath& path) {
    path_ = path.value();
    return *this;
  }

  const StringType& value() const { return path_;}
  FilePath AppendCopy(const StringType& path) const;
  void Append(const StringType& str);
  bool empty() const { return path_.empty();}

  static const StringType kRootPath;
 protected:
  StringType path_;
  static const CharType kSeperator;
};

AZER_EXPORT bool SplitPackage(const StringType& full, StringType* path,
                                StringType* package);
}  // namespace azer
