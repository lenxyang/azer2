#pragma once

#include "azer/render/entity.h"
#include "azer/effect/color_effect.h"

namespace azer {
class AxesFrame {
 public:
  AxesFrame();
  
  void Update(const Camera* camera);
  void SetColor(int32 index, const Vector3& color);
  void Render(Renderer* renderer);
 private:
  Vector4 colors_[3];
  scoped_ptr<AxisObject> object_; 
  DISALLOW_COPY_AND_ASSIGN(AxesFrame);
};

class ConeAxesFrame {
 public:
  ConeAxesFrame();
  
  void Update(Camera* camera);
  void SetColor(int32 index, const Vector3& color);
  void Render(Renderer* renderer);
 private:
  Vector4 colors_[3];
  scoped_ptr<AxisObject> object_; 
  DISALLOW_COPY_AND_ASSIGN(ConeAxesFrame);
};
}  // namespace azer
