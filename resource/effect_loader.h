#pragma once

#include "azer/base/export.h"
#include "azer/effect/light.h"
#include "azer/resource/scene_loader.h"

namespace azer {
class ConfigNode;

class EffectLoader : public ResourceSpecialLoader {
 public:
  static const char kSpecialLoaderName[];
  EffectLoader();
  ~EffectLoader();

  const char* GetLoaderName() const override;
  VariantResource Load(const ConfigNode* node, ResourceLoadContext* ctx) override;
  bool CouldLoad(ConfigNode* node) const override;
 private:
  VariantResource LoadFromLib(const ConfigNode* node, ResourceLoadContext* ctx);
  VariantResource LoadSource(const ConfigNode* node, ResourceLoadContext* ctx);
  DISALLOW_COPY_AND_ASSIGN(EffectLoader);
};

}  // namespace azer

