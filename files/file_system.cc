#include "azer/files/file_system.h"

#include "base/logging.h"

#include "azer/files/native_file_system.h"

namespace azer {
File::File(FileSystem* fs)
    : file_system_(fs) {
}

FileSystem* File::file_system() { return file_system_;}
const FileInfo& File::GetFileInfo() const { return file_info_;}
FileType File::GetType() const { return file_info_.type();}
int64 File::GetSize() const { return file_info_.size;}

FileSystem::FileSystem() {
}

}  // namespace azer
