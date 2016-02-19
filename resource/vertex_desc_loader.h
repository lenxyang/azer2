#pragma once

#include "azer/render/render.h"
#include "azer/render/light.h"
#include "azer/resource/scene_loader.h"

namespace azer {
class VertexDescLoader : public ResourceSpecialLoader {
 public:
  static const char kSpecialLoaderName[];
  VertexDescLoader();
  ~VertexDescLoader();

  const char* GetLoaderName() const override;
  VariantResource Load(const ConfigNode* node, ResourceLoadContext* ctx) override;
  bool CouldLoad(ConfigNode* node) const override;
 private:
  DISALLOW_COPY_AND_ASSIGN(VertexDescLoader);
};
}  // namespace azer

