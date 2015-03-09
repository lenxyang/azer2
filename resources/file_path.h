#pragma once

#include "base/files/file_path.h"
#include "azer/base/export.h"
#include "azer/base/string.h"

namespace azer {
class AZER_EXPORT ResFilePath {
 public:
  ResFilePath(const char* path);
  ResFilePath(const StringType& path) : path_(path) {}
  ResFilePath(const ResFilePath& path) : path_(path.value()) {}

  ResFilePath& operator = (const ResFilePath& path) {
    path_ = path.value();
    return *this;
  }

  const StringType& value() const { return path_;}
  ResFilePath AppendCopy(const StringType& path) const;
  void Append(const StringType& str);
  bool empty() const { return path_.empty();}
 protected:
  StringType path_;
  static const CharType kSeperator;
};

AZER_EXPORT bool SplitPackage(const StringType& full, StringType* path,
                                StringType* package);
}  // namespace azer
