#pragma once

#include "azer/base/res_path.h"
#include "azer/base/file_system.h"

namespace azer {
class AZER_EXPORT NativeFileSystem : public FileSystem {
 public:
  static const char kFileSystemName[];
  NativeFileSystem(const ::base::FilePath& root);
  ~NativeFileSystem() override {}

  const char* GetFileSystemName() const override;

  FileType GetFileType(const ResPath& path) override;
  bool IsPathExists(const ResPath& path) override;
  int64 GetFileSize(const ResPath& path) override;
  bool EnumDirectory(const ResPath& path, FileInfoVec* vec) override;
 private:
  bool ConvertFileSystem(const azer::ResPath& path, ::base::FilePath*);
  const ::base::FilePath root_;
  DISALLOW_COPY_AND_ASSIGN(NativeFileSystem);
};
}  // namespace azer
