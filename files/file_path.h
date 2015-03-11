#pragma once

#include "base/files/file_path.h"
#include "azer/base/export.h"
#include "azer/base/string.h"

namespace azer {
namespace files {
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
}  // namespace files
}  // namespace azer
