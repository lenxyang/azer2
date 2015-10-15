#include "azer/files/native_file_system.h"

#include "base/files/file_util.h"
#include "base/files/file_path.h"
#include "base/strings/string_util.h"
#include "azer/files/file_content.h"

namespace azer {
const char NativeFileSystem::kFileSystemName[] = "azer::NativeFileSystem";
NativeFileSystem::NativeFileSystem(const ::base::FilePath& root)
    : FileSystem(),
      root_(root) {
}

bool NativeFileSystem::ReadFile(const ResPath& path, std::vector<uint8>* data) {
  DCHECK(!path.empty());
  if (!path.IsAbsolutePath()) {
    return false;
  }

  ::base::FilePath real_path = root_.Append(path.filepath().substr(2).as_string());
  int64 size = 0;
  if (!::base::GetFileSize(real_path, &size)) {
    return false;
  }

  data->resize(size);
  int read = ::base::ReadFile(real_path, (char*)&((*data)[0]), size);
  if (size != read) {
    return false;
  }

  return true;
}

bool NativeFileSystem::IsPathExists(const azer::ResPath& path) {
  DCHECK(!path.empty());
  ::base::FilePath real_path;
  this->ConvertFileSystem(path, &real_path);
  return base::PathExists(real_path);
}

FileSystem::FileType NativeFileSystem::GetFileType(const ResPath& path) {
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

void NativeFileSystem::ReadFileAsync(const ResPath& path,
                                     std::vector<uint8>* data,
                                     ::base::Closure* callback) {
  NOTIMPLEMENTED();
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
