#pragma once

#include "azer/effect/light.h"
#include "azer/base/resource_loader.h"

namespace azer {
class MaterialLoader : public ResourceSpecialLoader {
 public:
  static const char kSpecialLoaderName[];
  MaterialLoader();
  ~MaterialLoader();

  const char* GetLoaderName() const override;
  VariantResource Load(const ConfigNode* node, ResourceLoadContext* ctx) override;
  bool CouldLoad(ConfigNode* node) const override;
 private:
  DISALLOW_COPY_AND_ASSIGN(MaterialLoader);
};
}  // namespace azer
