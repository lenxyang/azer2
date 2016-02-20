#pragma once

#include <string>
#include <map>
#include "azer/files/file_system.h"
#include "azer/files/res_path.h"

namespace azer {
class TestFileSystem : public FileSystem {
 public:
  TestFileSystem();
  ~TestFileSystem() override;

  FileContentPtr LoadFile(const ResPath& path) override;
  bool IsPathExists(const ResPath& path) override;
 private:
  std::map<StringType, FileContent> content;
  DISALLOW_COPY_AND_ASSIGN(TestFileSystem);
};
}  // namespace azer
