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
enum FileType {
  kDirectory,
  kArchiveFile,
  kSystemFile,
};

struct FileInfo {
  base::FilePath path;
  FileType type;
  int64 size;
};
typedef std::vector<FileInfo> FileInfoVec;

class AZER_EXPORT File : ::base::RefCountedThreadSafeBase<File> {
 public:
  explicit File(FileSystem* file_system);
  virtual ~File();

  FileSystem* file_system();
  const FileInfo& GetFileInfo() const;
  FileType GetType() const;
  int64 GetSize() const;

  virtual bool Open(const ResPath& path) = 0;
  virtual void Close() = 0;
  
  // Load content async
  // params: if size == -1, read the whole file
  virtual bool PRead(int64 offset, int64 size, std::vector<uint8>* content) = 0;
  virtual bool AsyncPRead(int64 offset, int64 size, std::vector<uint8>* content, 
                          base::Closure* callback) = 0;
 protected:
  FileSystem* file_system_;
  FileInfo file_info_;
  DISALLOW_COPY_AND_ASSIGN(File);
};

class AZER_EXPORT FileSystem {
 public:
  FileSystem();
  virtual ~FileSystem() {}

  virtual const char* GetFileSystemName() const = 0;

  virtual FilePtr CreateFileHandle() = 0;
  virtual FilePtr Open(const ResPath& path) = 0;
  virtual FileType GetFileType(const ResPath& path) = 0;
  virtual bool IsPathExists(const ResPath& path) = 0;
  virtual int64 GetFileSize(const ResPath& path) = 0;
  virtual bool EnumDirectory(const ResPath& path, FileInfoVec* vec) = 0;
 protected:
  DISALLOW_COPY_AND_ASSIGN(FileSystem);
};

typedef std::shared_ptr<FileSystem> FileSystemPtr;
}  // namespace azer
