#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace azer {
class ResourceLoaderManager;

class AZER_EXPORT ResourceContext {
 public:
  ResourceContext() {}
  virtual ~ResourceContext() {}

  // Get Resource Loader manager
  virtual ResourceLoaderManager* GetResourceLoaderManager() = 0;
 protected:
 private:
  DISALLOW_COPY_AND_ASSIGN(ResourceContext);
};
}  // namespace azer
