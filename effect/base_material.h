#pragma once

#include "azer/base/config_node.h"
#include "azer/render/render.h"

namespace azer {
class ConfigNode;
struct ResourceLoadContext;
class AZER_EXPORT BaseMaterial : public EffectParamsProvider {
 public:
  BaseMaterial() {}
  ~BaseMaterial() {}

  virtual bool Init(const ConfigNode* node, ResourceLoadContext* ctx) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(Material);
};

typedef scoped_refptr<BaseMaterial> BaseMaterialPtr;
}  // namespace lord
