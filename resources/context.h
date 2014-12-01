#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace azer {
namespace resource {

class ResourceLoaderCreator;

class AZER_EXPORT Context {
 public:
  virtual ~Context() {}
 protected:
  /**
   * create resource loader
   */
  virtual ResourceLoaderCreator* CreateResourceLoaderCreator();
 private:
  
  DISALLOW_COPY_AND_ASSIGN(Context);
};
}  // namespace resource
}  // namespace azer
