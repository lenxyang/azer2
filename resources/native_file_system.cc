#include "azer/resources/native_file_system.h"

#include "base/files/file_util.h"

namespace azer {
FileContent* NativeFileSystem::LoadFile(const ResFilePath& path) {
  DCHECK(!path.empty());
  ::base::FilePath real_path = fs_root_.Append(path.value());
  int64 size = 0;
  if (!::base::GetFileSize(real_path, &size)) {
    return NULL;
  }

  std::unique_ptr<char[]> content(new char[size - 1 + sizeof(FileContent)]);
  if (size != ::base::ReadFile(real_path,
                               content.get() + offsetof(FileContent, data),
                               size)) {
    return NULL;
  }

  FileContent* cptr = ((FileContent*)content.release());
  cptr->length = size;
  return cptr;
}

bool NativeFileSystem::IsPathExists(const ResFilePath& path) {
  DCHECK(!path.empty());
  ::base::FilePath real_path = fs_root_.Append(path.value());
  return base::PathExists(real_path);
}
}  // namespace azer
