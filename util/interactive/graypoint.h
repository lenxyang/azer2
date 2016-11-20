#pragma once

#include "azer/math/math.h"
#include "azer/render/entity.h"
#include "azer/render/effect.h"

namespace azer {
class Camera;
class TextBillboardEffect;
class TextureView;

class GrayPoint {
 public:
  GrayPoint();
  ~GrayPoint();
  
  const Vector3& position() const { return position_;}
  void set_position(const Vector3& pos) { position_ = pos;}
  void set_scale(const Vector3& v) { scale_ = v;}
  void set_color(const Vector4& c) { color_ = c;}
  void Render(const Camera& camera, Renderer* renderer);
 private:
  Vector3 position_;
  Vector3 scale_;
  Vector4 color_;
  EntityPtr point_;
  scoped_refptr<TextureView> pointgray_texview_;
  scoped_refptr<TextBillboardEffect> effect_;
  DISALLOW_COPY_AND_ASSIGN(GrayPoint);
};
}  // namespace azer
