#pragma once

#include "azer/render/vertex_buffer.h"
#include "azer/render/geometry/geometry_object.h"

namespace azer {
class Camera;
class AZER_EXPORT FrustrumObject : public GeometryObject {
 public:
  FrustrumObject(const Camera* camera, VertexDescPtr desc);
  ~FrustrumObject();

  void Render(Renderer* renderer) override;
  MeshPartPtr CreateFrameObject(Effect* effect) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(FrustrumObject);
};
}   // namespace azer