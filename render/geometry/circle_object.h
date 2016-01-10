#pragma once

#include "azer/render/geometry/geometry_object.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/index_pack.h"

namespace azer {

class AZER_EXPORT CircleObject : public GeometryObject {
 public:
  CircleObject(VertexDescPtr desc, int32 slice = 32, 
               float radius = 1.0f, float y = 0.0f);
  ~CircleObject();

  void Render(Renderer* renderer) override;
  void RenderWireframe(Renderer* renderer) override;
 private:
  void InitHardwareBuffers();
  const int32 kSlice;
  float radius_;
  float y_;
  DISALLOW_COPY_AND_ASSIGN(CircleObject);
};

}  // namespace azer
