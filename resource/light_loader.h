#pragma once

#include "azer/effect/light.h"
#include "azer/resource/scene_loader.h"

namespace azer {
class LightLoader : public ResourceSpecialLoader {
 public:
  static const char kSpecialLoaderName[];
  LightLoader();
  ~LightLoader();

  const char* GetLoaderName() const override;
  VariantResource Load(const ConfigNode* node, ResourceLoadContext* ctx) override;
  bool CouldLoad(ConfigNode* node) const override;
 private:
  DISALLOW_COPY_AND_ASSIGN(LightLoader);
};
}  // namespace azer
