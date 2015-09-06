#pragma once

#include <memory>
#include <vector>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "azer/base/export.h"
#include "azer/base/string.h"
#include "azer/files/res_path.h"
#include "azer/files/file_content.h"

namespace azer {
class AZER_EXPORT FileSystem {
 public:
  FileSystem();
  virtual ~FileSystem() {}

  virtual const char* GetFileSystemName() const = 0;

  enum FileType {
    kDirectory,
    kArchiveFile,
    kSystemFile,
  };
  virtual FileType GetFileType(const ResPath& path) = 0;
  virtual bool IsPathExists(const ResPath& path) = 0;
  virtual int64 GetFileSize(const ResPath& path) = 0;

  struct FileInfo {
    base::FilePath path;
    FileType type;
    int64 size;
  };
  typedef std::vector<FileInfo> FileInfoVec;
  virtual bool EnumDirectory(const ResPath& path, FileInfoVec* vec) = 0;

  // Load content async
  virtual bool ReadFile(const ResPath& path, std::vector<uint8>* content) = 0;
  virtual void ReadFileAsync(const ResPath& path, std::vector<uint8>* content,
                             base::Closure* callback) = 0;
 protected:
  DISALLOW_COPY_AND_ASSIGN(FileSystem);
};

typedef std::shared_ptr<FileSystem> FileSystemPtr;
}  // namespace azer
