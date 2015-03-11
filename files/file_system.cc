#include "azer/files/file_system.h"

#include "base/logging.h"

#include "azer/files/native_file_system.h"

namespace azer {

FileSystem* FileSystem::create(FileSystem::Type type, 
                               const ::base::FilePath& root) {
  switch(type) {
    case kNativeFS:
      return new files::NativeFileSystem(root); 
    default:
      NOTREACHED();
      return NULL;
  }
}
}  // namespace azer
