#include "azer/base/native_file_system.h"

#include "base/files/file_util.h"
#include "base/files/file_path.h"
#include "base/strings/string_util.h"

namespace azer {
NativeFile::NativeFile(NativeFileSystem* nfs) 
    : File(nfs),
      native_fs_(nfs) {
}

bool NativeFile::Open(const ResPath& path) {
  DCHECK(!file_.get());
  DCHECK(!path.empty());
  if (!path.IsAbsolutePath()) {
    return false;
  }

  const ::base::FilePath& root = native_fs_->GetRootPath();
  real_path_ = root.Append(path.filepath().substr(2).as_string());
  file_.reset(new base::File(
      real_path_, base::File::FLAG_OPEN | base::File::FLAG_READ));

  return file_->IsValid();;
}

void NativeFile::Close() {
}

bool NativeFile::PRead(int64 offset, int64 size, FileContents* contents) {
  DCHECK(file_.get());

  base::File::Info info;
  if (!file_->GetInfo(&info)) {
    LOG(ERROR) << "Failed to get path \"" << real_path_.value() << "\"'s info";
    return false;
  }

  int64 expected_size = 0;
  if (size >= 0) {
    expected_size = std::min(size, info.size - offset);
  } else {
    expected_size = info.size - offset;
  }

  contents->resize(expected_size);
  int read = file_->Read(offset, (char*)&((*contents)[0]), expected_size);
  if (expected_size != read) {
    return false;
  }

  return true;
}

bool NativeFile::AsyncPRead(int64 offset, int64 size, FileContents* content, 
                            base::Closure* callback) {
  NOTIMPLEMENTED();
  return true;
}

// class NativeFileSystem
const char NativeFileSystem::kFileSystemName[] = "azer::NativeFileSystem";
NativeFileSystem::NativeFileSystem(const ::base::FilePath& root)
    : FileSystem(),
      root_(root) {
}

const char* NativeFileSystem::GetFileSystemName() const {
  return kFileSystemName;
}

FilePtr NativeFileSystem::OpenFile(const ResPath& path) {
  FilePtr ptr(new NativeFile(this));
  if (ptr->Open(path)) {
    return ptr;
  } else {
    return FilePtr();
  }
}

bool NativeFileSystem::IsPathExists(const azer::ResPath& path) {
  DCHECK(!path.empty());
  ::base::FilePath real_path;
  this->ConvertFileSystem(path, &real_path);
  return base::PathExists(real_path);
}

FileType NativeFileSystem::GetFileType(const ResPath& path) {
  return kArchiveFile;
}

int64 NativeFileSystem::GetFileSize(const ResPath& path) {
  NOTIMPLEMENTED();
  return -1;
}

bool NativeFileSystem::EnumDirectory(const ResPath& path, FileInfoVec* vec) {
  NOTIMPLEMENTED();
  return false;
}

bool NativeFileSystem::ConvertFileSystem(const azer::ResPath& path,
                                         ::base::FilePath* realpath) {
  if (!path.IsAbsolutePath())
    return false;

  StringType realpathstr = root_.value();
  realpathstr.append(FILE_PATH_LITERAL("/"));
  realpathstr.append(path.filepath().substr(2).as_string());
  *realpath = ::base::FilePath(realpathstr);
  return true;
}
}  // namespace azer
