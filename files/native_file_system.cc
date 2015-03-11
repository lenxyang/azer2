#include "azer/files/native_file_system.h"

#include "base/files/file_util.h"
#include "base/files/file_path.h"

#include "azer/files/file_content.h"

namespace azer {
namespace files {
FileContentPtr NativeFileSystem::LoadFile(const ResFilePath& path) {
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

bool NativeFileSystem::IsPathExists(const ResFilePath& path) {
  DCHECK(!path.empty());
  ::base::FilePath real_path = fs_root_.Append(path.value());
  return base::PathExists(real_path);
}
}  // namespace files
}  // namespace azer
