#pragma once

#include "azer/resources/file_system.h"

namespace azer {

class AZER_EXPORT NativeFileSystem : public FileSystem {
 public:
  NativeFileSystem(const ::base::FilePath& root)
      : FileSystem(FileSystem::kNativeFS, root) {
  }

  ~NativeFileSystem() override {}

  FileContent* LoadFile(const ResFilePath& path) override;
  bool IsPathExists(const ResFilePath& path) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(NativeFileSystem);
};

}  // namespace azer
