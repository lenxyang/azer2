#include "azer/files/native_file_system.h"

#include "base/files/file_util.h"
#include "base/files/file_path.h"
#include "base/strings/string_util.h"
#include "azer/files/file_content.h"

namespace azer {
namespace files {
FileContentPtr NativeFileSystem::LoadFile(const ResPath& path) {
  DCHECK(!path.empty());
  if (!path.IsAbsolutePath()) {
    return FileContentPtr();
  }

  ::base::FilePath real_path = fs_root_.Append(path.filepath().substr(2));
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

bool NativeFileSystem::ConvertFileSystem(const azer::ResPath& path,
                                         ::base::FilePath* realpath) {
  if (!path.IsAbsolutePath()) { return false;}

  StringType realpathstr = root().value();
  realpathstr.append(FILE_PATH_LITERAL("/"));
  realpathstr.append(path.filepath().substr(2));
  *realpath = ::base::FilePath(realpathstr);
  return true;
}

FileSystem::FileType NativeFileSystem::GetFileType(const ResPath& path) {
  return kArchiveFile;
}
}  // namespace files
}  // namespace azer
