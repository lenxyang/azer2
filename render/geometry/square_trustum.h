#pragma once

#include "azer/render/geometry/geometry_object.h"

namespace azer {
class AZER_EXPORT SquareTrustum : public GeometryObject {
 public:
  SquareTrustum(VertexDescPtr desc, float top, float bottom, float height);
  ~SquareTrustum();

  virtual void Render(Renderer* renderer);
  virtual void RenderWireframe(Renderer* renderer);
 private:
  void InitHardwareBuffers();
  float top_length_;
  float bottom_length_;
  float height_;
  DISALLOW_COPY_AND_ASSIGN(SquareTrustum);
};
}  // namespace azer
