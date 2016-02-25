#pragma once

#include "azer/effect/base_material.h"
#include "azer/math/vector4.h"
#include "azer/render/texture.h"

namespace azer {
struct ColorMaterialData {
  Vector4  diffuse;
  Vector4  ambient;
  Vector4  specular;
  Vector4  emission;
  float    specular_power;
  float    alpha;
  float    padding1;
  float    padding2;

  ColorMaterialData();
};

struct DiffuseMapMaterialData {
  TexturePtr diffusemap;
  float ambient_scalar;
  float specular_scalar;
  float alpha;

  DiffuseMapMaterialData();
};

struct TextureMaterialData {
  TexturePtr diffusemap;
  TexturePtr specularmap;
  TexturePtr emissionmap;
  TexturePtr alphamap;
  TexturePtr normalmap;
  float ambient_scalar;
  float specular_scalar;
  float alpha;

  TextureMaterialData();
};

class DiffuseMapMaterial : public BaseMaterial {
 public:
  static const char kEffectProviderName[];
  const char* GetProviderName() const override { return kEffectProviderName;}
  DiffuseMapMaterial();
  ~DiffuseMapMaterial() override;

  DiffuseMapMaterialData* mutable_data() { return &data_;}
  const DiffuseMapMaterialData& data() const { return data_;}
  bool Init(const ConfigNode* node, ResourceLoadContext* ctx) override;
  static EffectParamsProvider* CreateObject() { return new DiffuseMapMaterial;}
 private:
  DiffuseMapMaterialData data_;
  DISALLOW_COPY_AND_ASSIGN(DiffuseMapMaterial);
};

class TextureMaterial : public BaseMaterial {
 public:
  static const char kEffectProviderName[];
  const char* GetProviderName() const override { return kEffectProviderName;}
  TextureMaterial();
  ~TextureMaterial() override;

  void SetData(const TextureMaterialData& data) { data_ = data;}
  TextureMaterialData* mutable_data() { return &data_;}
  const TextureMaterialData& data() const { return data_;}
  bool Init(const ConfigNode* node, ResourceLoadContext* ctx) override;
  static EffectParamsProvider* CreateObject() { return new TextureMaterial;}
 private:
  TextureMaterialData data_;
  DISALLOW_COPY_AND_ASSIGN(TextureMaterial);
};

}  // namespace azer
