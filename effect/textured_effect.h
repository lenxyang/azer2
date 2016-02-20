#pragma once

#include "azer/render/effect_creator.h"
#include "azer/render/render.h"
#include "azer/render/light.h"
#include "azer/scene/render_node.h"

namespace azer {
class TexturedEffect : public Effect {
 public:
  static const char kEffectName[];
  TexturedEffect();
  ~TexturedEffect();

  const char* GetEffectName() const override;
#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pv;
    Matrix4 world;
    Vector4 camerapos;
  };

  struct ps_cbuffer {
    UniverseLight lights[4];
  };
#pragma pack(pop)

  void SetPV(const Matrix4& value);
  void SetWorld(const Matrix4& value);
  void SetCameraPos(const Vector4& CameraPos);
  void SetLights(const LightPtr* value, int32 count);
  void set_ambient_scalar(float scalar) { ambient_scalar_ = scalar;}
  void set_specular_scalar(float scalar) { specular_scalar_ = scalar;}
  void set_diffuse_texture(Texture* tex) { diffuse_map_ = tex;}
  void set_alpha(float alpha) { alpha_ = alpha;}

  static Effect* CreateObject() { return new TexturedEffect;}
 protected:
  void UseTexture(Renderer* renderer) override;
  void ApplyGpuConstantTable(Renderer* renderer) override;
  void InitGpuConstantTable() override;

  Matrix4 pv_;
  Matrix4 world_;
  Vector4 camerapos_;
  TexturePtr diffuse_map_;
  float ambient_scalar_;
  float specular_scalar_;
  float alpha_;
  UniverseLight lights_[4];
  DECLARE_EFFECT_DYNCREATE(TexturedEffect);
  DISALLOW_COPY_AND_ASSIGN(TexturedEffect);
};


}  // namespace azer
