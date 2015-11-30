#include "azer/base/file_system.h"

#include "base/logging.h"
#include "azer/base/res_path.h"

namespace azer {
File::File(FileSystem* fs)
    : file_system_(fs) {
}

File::~File() {
}

FileSystem* File::file_system() {
  return file_system_;
}

const FileInfo& File::GetFileInfo() const {
  return file_info_;
}

FileType File::GetType() const {
  return file_info_.type;
}

int64 File::GetSize() const {
  return file_info_.size;
}

FileSystem::FileSystem() {
}

bool LoadFileContents(const ResPath& path, FileContents* contents, FileSystem* fs) {
  FilePtr file = fs->OpenFile(path);
  if (!file.get()) {
    LOG(ERROR) << "Failed to open file: " << path.fullpath();
    return false;
  }

  if (!file->PRead(0, -1, contents)) {
    LOG(ERROR) << "Failed to read file: " << path.fullpath();
    return false;
  }
  return true;
}
}  // namespace azer
