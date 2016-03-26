#pragma once

#include "azer/math/vector3.h"
#include "azer/math/vector4.h"
#include "azer/render/entity.h"

namespace azer {
class Camera;
class ColorEffect;
class AmbientColorEffect;
class Texture;
class TextBillboardEffect;

class AxesFrame {
 public:
  AxesFrame();
  ~AxesFrame();
  
  void set_position(const Vector3& v) { position_ = v;}
  void set_scale(const Vector3& v) { scale_ = v;}
  void SetColor(int32 index, const Vector4& color);
  void Update(const Camera* camera);
  void SetWorld(const Matrix4& world);
  void Render(Renderer* renderer);
 private:
  Vector3 scale_;
  Vector3 position_;
  Vector4 colors_[3];
  scoped_refptr<Texture> tex_[3];
  scoped_refptr<AmbientColorEffect> effect_;
  scoped_refptr<TextBillboardEffect> texeffect_;
  EntityPtr entity_;
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
  scoped_refptr<ColorEffect> effect_;
  DISALLOW_COPY_AND_ASSIGN(ConeAxesFrame);
};
}  // namespace azer
