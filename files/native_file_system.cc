#include "azer/files/native_file_system.h"

#include "base/files/file_util.h"
#include "base/files/file_path.h"

#include "azer/files/file_content.h"

namespace azer {
namespace files {
FileContentPtr NativeFileSystem::LoadFile(const FilePath& path) {
  DCHECK(!path.empty());
  ::base::FilePath real_path = fs_root_.Append(path.value());
  int64 size = 0;
  if (!::base::GetFileSize(real_path, &size)) {
    return NULL;
  }

  FileContentPtr content(new FileContent(size));
  int read = ::base::ReadFile(real_path, (char*)content->data(),
                                content->capability());
  if (size != read) {
    return NULL;
  }

  return content;
}

bool NativeFileSystem::IsPathExists(const FilePath& path) {
  DCHECK(!path.empty());
  ::base::FilePath real_path;
  ConvertFileSystem(path, &realpath);
  return base::PathExists(real_path);
}

void FileSystem::ConvertFileSystem(FilePath path, ::base::FilePath* realpath) {
  CHECK(StartsWith(path.value(), FILE_PATH_LITERAL("//"), true));
  StringType realpathstr = root().value();
  realpathstr.append(FILE_PATH_LITERAL("/"));
  realpathstr.append(path.substr(2));
  *realpath = realpathstr;
}
}  // namespace files
}  // namespace azer
