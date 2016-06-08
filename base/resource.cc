#include "azer/base/resource.h"

namespace azer {
VariantResource::VariantResource() : type(kResTypeNone), retcode(-1) {}

int32_t GetTypeFromString(const std::string& str) {
  if (str == "mesh") {
    return kResTypeMesh;
  } else if (str == "material") {
    return kResTypeMaterial;
  } else if (str == "effect") {
    return kResTypeEffect;
  } else if (str == "light") {
    return kResTypeLight;
  } else if (str == "scene") {
    return kResTypeScene;
  } else if (str == "vertex_desc") {
    return kResTypeVertexDesc;
  } else {
    NOTREACHED() << "unknown type: " << str;
    return kResTypeNone;
  }
}
}  // namespace azer
