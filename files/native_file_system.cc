#include "azer/files/native_file_system.h"

#include "base/files/file_util.h"
#include "base/files/file_path.h"
#include "base/strings/string_util.h"
#include "azer/files/file_content.h"

namespace azer {
namespace files {
FileContentPtr NativeFileSystem::LoadFile(const ResPath& path) {
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

bool NativeFileSystem::IsPathExists(const azer::ResPath& path) {
  DCHECK(!path.empty());
  ::base::FilePath real_path;
  this->ConvertFileSystem(path, &real_path);
  return base::PathExists(real_path);
}

void NativeFileSystem::ConvertFileSystem(const azer::ResPath& path,
                                         ::base::FilePath* realpath) {
  CHECK(StartsWith(path.value(), FILE_PATH_LITERAL("//"), true));
  StringType realpathstr = root().value();
  realpathstr.append(FILE_PATH_LITERAL("/"));
  realpathstr.append(path.filepath().substr(2));
  *realpath = ::base::FilePath(realpathstr);
}

FileType NativeFileSystem::GetFileType(const ResPath& path) {
  return kArchiveFile
}
}  // namespace files
}  // namespace azer
