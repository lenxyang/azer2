#pragma once

#include "azer/render/vertex_buffer.h"
#include "azer/render/geometry/geometry_object.h"

namespace azer {
class Camera;
class AZER_EXPORT FrustumObject : public GeometryObject {
 public:
  FrustumObject(const Camera* camera, VertexDescPtr desc);
  ~FrustumObject();

  void Render(Renderer* renderer) override;
  MeshPartPtr CreateFrameObject(Effect* effect) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(FrustumObject);
};
}   // namespace azer
