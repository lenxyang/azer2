#pragma once

#include "azer/base/res_path.h"
#include "azer/base/file_system.h"

namespace azer {
class NativeFileSystem;

class NativeFile : public File {
 public:
  explicit NativeFile(NativeFileSystem* nfs);
  bool Open(const ResPath& path) override;
  void Close() override;

  bool PRead(int64 offset, int64 size, FileContents* content) override;
  bool AsyncPRead(int64 offset, int64 size, FileContents* content, 
                  base::Closure* callback) override;
 private:
  NativeFileSystem* native_fs_;
  base::FilePath real_path_;
  scoped_ptr<::base::File> file_;
  DISALLOW_COPY_AND_ASSIGN(NativeFile);
};

class AZER_EXPORT NativeFileSystem : public FileSystem {
 public:
  static const char kFileSystemName[];
  NativeFileSystem(const ::base::FilePath& root);
  ~NativeFileSystem() override {}

  const char* GetFileSystemName() const override;

  FilePtr OpenFile(const ResPath& path) override;
  FileType GetFileType(const ResPath& path) override;
  bool IsPathExists(const ResPath& path) override;
  int64 GetFileSize(const ResPath& path) override;
  bool EnumDirectory(const ResPath& path, FileInfoVec* vec) override;

  const base::FilePath& GetRootPath() const { return root_;}
 private:
  bool ConvertFileSystem(const azer::ResPath& path, ::base::FilePath*);
  const ::base::FilePath root_;
  DISALLOW_COPY_AND_ASSIGN(NativeFileSystem);
};
}  // namespace azer
