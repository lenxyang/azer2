#pragma once

#include "azer/render/geometry/geometry_object.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/index_pack.h"

namespace azer {

class CircleObject : public GeometryObject {
 public:
  CircleObject(VertexDescPtr desc);
  CircleObject(VertexDescPtr desc, int32 slice);
  ~CircleObject();

  void Render(Renderer* renderer) override;
  void RenderWireframe(Renderer* renderer) override;
 private:
  void InitHardwareBuffers();
  const int32 kSlice;
  DISALLOW_COPY_AND_ASSIGN(CircleObject);
};

}  // namespace azer
