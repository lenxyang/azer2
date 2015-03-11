#pragma once

#include "azer/files/file_path.h"
#include "azer/files/file_system.h"

namespace azer {
namespace files {
class AZER_EXPORT NativeFileSystem : public FileSystem {
 public:
  NativeFileSystem(const ::base::FilePath& root)
      : FileSystem(FileSystem::kNativeFS, root) {
  }

  ~NativeFileSystem() override {}

  FileContentPtr LoadFile(const azer::FilePath& path) override;
  bool IsPathExists(const azer::FilePath& path) override;
 private:
  void ConvertFileSystem(const azer::FilePath& path, ::base::FilePath) override;
  DISALLOW_COPY_AND_ASSIGN(NativeFileSystem);
};
}  // namespace files
}  // namespace azer
