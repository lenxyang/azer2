#pragma once

#include "azer/base/export.h"
#include "azer/effect/light.h"
#include "azer/resource/resource_loader.h"

namespace azer {
class MeshLoader : public ResourceSpecialLoader {
 public:
  static const char kSpecialLoaderName[];
  MeshLoader();
  ~MeshLoader();

  const char* GetLoaderName() const override;
  VariantResource Load(const ConfigNode* node, ResourceLoadContext* ctx) override;
  bool CouldLoad(ConfigNode* node) const override;
 private:
  MeshPtr LoadMeshData(const ConfigNode* node, VertexDesc* desc,
                       ResourceLoadContext* ctx);
  DISALLOW_COPY_AND_ASSIGN(MeshLoader);
};
}  // namespace azer
