#pragma once

#include "azer/render/util/geometry/geometry_object.h"

namespace azer {

class BoxObject : public GeometryObject {
 public:
  BoxObject(VertexDescPtr desc);
  ~BoxObject();

  void Render(Renderer* renderer) override;
  MeshPartPtr CreateFrameObject(Effect* effect) override;
 private:
  void InitHardwareBuffers();
  DISALLOW_COPY_AND_ASSIGN(BoxObject);
};
}  // namespace azer
