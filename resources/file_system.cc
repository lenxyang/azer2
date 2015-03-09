#include "azer/resources/file_system.h"

#include "base/logging.h"

#include "azer/resources/native_file_system.h"

namespace azer {

FileSystem* FileSystem::create(FileSystem::Type type, 
                               const ::base::FilePath& root) {
  switch(type) {
    case kNativeFS:
      return new NativeFileSystem(root); 
    default:
      NOTREACHED();
      return NULL;
  }
}

ResFilePath::ResFilePath(const char* path) {
  ConvertPath<StringType>(path, &path_);
}

namespace {
FileSystem* comm_res_fs_ = NULL;
}  // namespace

void FileSystem::InitDefaultFileSystem(const ::base::FilePath& root, Type type) {
  CHECK(comm_res_fs_ == NULL);
  comm_res_fs_ = create(type, root);
}

FileSystem* FileSystem::GetDefaultFileSystem() {
  CHECK(comm_res_fs_ != NULL);
  return comm_res_fs_;
}

void FileSystem::InitDefaultFileSystem(const StringType& root, Type type) {
  InitDefaultFileSystem(::base::FilePath(root), type);
}

}  // namespace azer
