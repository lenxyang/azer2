#include "azer/resource/context.h"

#include "azer/base/resource.h"
#include "azer/resource/resource_loader.h"
#include "base/logging.h"

namespace azer {
namespace resource {
ResourceLoaderCreator* Context::CreateResourceLoaderCreator() {
  return NULL;
}
}  // namespace resource
}  // namespace azer
