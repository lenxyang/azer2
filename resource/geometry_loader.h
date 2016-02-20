#pragma once

#include "azer/effect/light.h"
#include "azer/resource/resource_loader.h"

namespace azer {
class GeometryLoader : public ResourceSpecialLoader {
 public:
  static const char kSpecialLoaderName[];
  GeometryLoader();
  ~GeometryLoader();

  const char* GetLoaderName() const override;
  VariantResource Load(const ConfigNode* node, ResourceLoadContext* ctx) override;
  bool CouldLoad(ConfigNode* node) const override;
 private:
  MeshPartPtr CreateSphere(const ConfigNode* node, Effect*, ResourceLoadContext*);
  MeshPartPtr CreateBox(const ConfigNode* node, Effect*, ResourceLoadContext* );
  MeshPartPtr CreatePlane(const ConfigNode* node, Effect*, ResourceLoadContext* );
  MeshPartPtr CreateCone(const ConfigNode* node, Effect*, ResourceLoadContext* );
  MeshPartPtr CreateRound(const ConfigNode* node, Effect*, ResourceLoadContext* );
  MeshPartPtr CreateCylinder(const ConfigNode* node, Effect*, ResourceLoadContext* );
  MeshPartPtr CreateBarrel(const ConfigNode* node, Effect*, ResourceLoadContext* );
  MeshPartPtr CreateTour(const ConfigNode* node, Effect*, ResourceLoadContext* );
  DISALLOW_COPY_AND_ASSIGN(GeometryLoader);
};
}  // namespace azer
