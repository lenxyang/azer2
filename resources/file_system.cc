#include "azer/resources/file_system.h"

#include "base/logging.h"

#include "azer/resources/native_file_system.h"

namespace azer {
namespace resources {

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
}  // namespace resources
}  // namespace azer
