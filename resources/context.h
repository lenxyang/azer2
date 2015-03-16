#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace azer {
class FileSystem;

namespace resources {
class ResourceLoaderManager;
}  // namespace resources

class AZER_EXPORT ResourceContext {
 public:
  ResourceContext() {}
  virtual ~ResourceContext() {}

  // Get Resource Loader manager
  virtual resources::ResourceLoaderManager* GetResourceLoaderManager() = 0;

  virtual FileSystem* GetFileSystem() = 0;
 protected:
  DISALLOW_COPY_AND_ASSIGN(ResourceContext);
};
}  // namespace azer
