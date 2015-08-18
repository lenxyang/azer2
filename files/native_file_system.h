#pragma once

#include "azer/files/res_path.h"
#include "azer/files/file_system.h"

namespace azer {
namespace files {
class AZER_EXPORT NativeFileSystem : public FileSystem {
 public:
  static const char kFileSystemName[];
  NativeFileSystem(const ::base::FilePath& root);
  ~NativeFileSystem() override {}

  FileType GetFileType(const ResPath& path) override;
  bool IsPathExists(const ResPath& path) override;
  int64 GetFileSize(const ResPath& path) override;
  bool EnumDirectory(const ResPath& path, FileInfoVec* vec) override;

  FileContentPtr LoadFile(const azer::ResPath& path) override;
  void ResLoadFileAsync(const ResPath& path, FileContent* filecontent,
                        ::base::Closure* callback) override;
 private:
  bool ConvertFileSystem(const azer::ResPath& path, ::base::FilePath*);
  const ::base::FilePath root_;
  DISALLOW_COPY_AND_ASSIGN(NativeFileSystem);
};
}  // namespace files
}  // namespace azer
