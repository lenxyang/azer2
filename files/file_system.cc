#include "azer/files/file_system.h"

#include "base/logging.h"

#include "azer/files/native_file_system.h"

namespace azer {
namespace files {

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
}  // namespace files
}  // namespace azer
