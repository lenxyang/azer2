#pragma once

#include "azer/render/geometry/geometry_object.h"

namespace azer {

class AZER_EXPORT BoxObject : public GeometryObject {
 public:
  BoxObject(VertexDescPtr desc);
  ~BoxObject();

  void Render(Renderer* renderer) override;
  MeshPartPtr CreateFrameObject(Effect* effect) override;
 private:
  void InitHardwareBuffers();
  DISALLOW_COPY_AND_ASSIGN(BoxObject);
};

SlotVertexDataPtr AZER_EXPORT CreateBoxVertexData(VertexDesc* desc);
IndicesDataPtr AZER_EXPORT CreateBoxFrameIndicesData();
}  // namespace azer
