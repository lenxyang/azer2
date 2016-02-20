#pragma once

#include "base/memory/ref_counted.h"
#include "azer/effect/effect_params_provider.h"

namespace azer {
class ConfigNode;
struct ResourceLoadContext;
class AZER_EXPORT BaseMaterial : public EffectParamsProvider {
 public:
  BaseMaterial() {}
  ~BaseMaterial() {}

  virtual bool Init(const ConfigNode* node, ResourceLoadContext* ctx) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(BaseMaterial);
};

typedef scoped_refptr<BaseMaterial> BaseMaterialPtr;
}  // namespace lord
