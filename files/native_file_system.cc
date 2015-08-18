#include "azer/files/native_file_system.h"

#include "base/files/file_util.h"
#include "base/files/file_path.h"
#include "base/strings/string_util.h"
#include "azer/files/file_content.h"

namespace azer {
namespace files {
const char NativeFileSystem::kFileSystemName[] = "azer::NativeFileSystem";
NativeFileSystem::NativeFileSystem(const ::base::FilePath& root)
    : FileSystem(),
      root_(root) {
}

FileContentPtr NativeFileSystem::LoadFile(const ResPath& path) {
  DCHECK(!path.empty());
  if (!path.IsAbsolutePath()) {
    return FileContentPtr();
  }

  ::base::FilePath real_path = root_.Append(path.filepath().substr(2).as_string());
  int64 size = 0;
  if (!::base::GetFileSize(real_path, &size)) {
    return FileContentPtr();
  }

  scoped_ptr<uint8[]> data(new uint8[size]);
  int read = ::base::ReadFile(real_path, (char*)data.get(), size);
  if (size != read) {
    return FileContentPtr();
  }

  FileContentPtr ptr(new FileContent(data.Pass(), size));
  return ptr;
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

void NativeFileSystem::ResLoadFileAsync(const ResPath& path,
                                        FileContent* filecontent,
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
}  // namespace files
}  // namespace azer
