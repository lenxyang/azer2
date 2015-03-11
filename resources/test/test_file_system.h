#pragma once

#include <string>
#include <map>
#include "azer/resources/file_system.h"
#include "azer/resources/file_path.h"

namespace azer {
class TestFileSystem : public FileSystem {
 public:
  TestFileSystem();
  ~TestFileSystem() override;

  FileContent* LoadFile(const ResFilePath& path) override;
  bool IsPathExists(const ResFilePath& path) override;
 private:
  std::map<StringType, FileContent> content;
  DISALLOW_COPY_AND_ASSIGN(TestFileSystem);
};
}  // namespace azer
