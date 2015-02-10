#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace azer {
class ResourceLoaderCreator;

class AZER_EXPORT ResourceContext {
 public:
  virtual ~ResourceContext() {}
 protected:
  /**
   * create resource loader
   */
  virtual ResourceLoaderCreator* CreateResourceLoaderCreator();
 private:
  DISALLOW_COPY_AND_ASSIGN(ResourceContext);
};
}  // namespace azer
