#pragma once

#include "azer/effect/effect.h"

namespace azer {
class TextBillboardEffect;

class GrayPoint {
 public:
  GrayPoint();
  
  const Vector3& position() const { return position_;}
  void set_position(const Vector3& pos) { position_ = pos;}
  void set_scale(const Vector3& v) { scale_ = v;}
  void set_color(const Vector4& c) { color_ = c;}
 private:
  Vector3 position_;
  Vector3 scale_;
  Vector4 color_;
  scoped_refptr<TextBillboardEffect> effect_;
  DISALLOW_COPY_AND_ASSIGN(GrayPoint);
};
}  // namespace azer
