#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/files/res_path.h"
#include "azer/files/file_content.h"

namespace azer {
class FileSystem;
class ResourceLoaderManager;

class AZER_EXPORT ResourceContext {
 public:
  ResourceContext() {}
  virtual ~ResourceContext() {}

  // Get Resource Loader manager
  virtual ResourceLoaderManager* GetResourceLoaderManager() = 0;

  virtual FileContentPtr LoadFile(const ResPath& path) = 0;
 protected:
  DISALLOW_COPY_AND_ASSIGN(ResourceContext);
};
}  // namespace azer
