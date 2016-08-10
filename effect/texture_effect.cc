#include "azer/effect/texture_effect.h"

#include "base/strings/utf_string_conversions.h"
#include "azer/azer.h"


namespace azer {
using base::UTF8ToUTF16;

IMPLEMENT_EFFECT_DYNCREATE(TextureEffect);
const char TextureEffect::kEffectName[] = "azer::TextureEffect";
TextureEffect::TextureEffect() : light_count_(0) {}
TextureEffect::~TextureEffect() {}
const char* TextureEffect::GetEffectName() const { return kEffectName;}

ShaderClosurePtr TextureEffect::InitShaderClosure(RenderPipelineStage stage,
                                                  Shader* shader) {
  RenderSystem* rs = RenderSystem::Current();
  ShaderParamTablePtr table;
  ShaderClosurePtr closure(new ShaderClosure(stage));
  if (stage == kVertexStage) {
    // generate GpuTable init for stage kVertexStage
    ShaderParamTable::Desc vs_table_desc[] = {
      ShaderParamTable::Desc("pv", ShaderParamType::kMatrix4,
                              offsetof(vs_cbuffer, pv), 1),
      ShaderParamTable::Desc("world", ShaderParamType::kMatrix4,
                              offsetof(vs_cbuffer, world), 1),
      ShaderParamTable::Desc("camerapos", ShaderParamType::kVector4,
                              offsetof(vs_cbuffer, camerapos), 1),
    };
    table = rs->CreateShaderParamTable(arraysize(vs_table_desc), vs_table_desc);
    closure->SetShaderParamTable(kVertexStage, 0, table.get());
    closure->SetShader(shader, 1, 0, 0);
  } else if (stage == kPixelStage) {
    // generate GpuTable init for stage kPixelStage
    ShaderParamTable::Desc ps_table_desc[] = {
      ShaderParamTable::Desc("ambient_scalar", ShaderParamType::kFloat,
                              offsetof(ps_cbuffer, ambient_scalar), 1),
      ShaderParamTable::Desc("specular_scalar", ShaderParamType::kFloat,
                              offsetof(ps_cbuffer, specular_scalar), 1),
      ShaderParamTable::Desc("alpha", ShaderParamType::kFloat,
                              offsetof(ps_cbuffer, alpha), 1),
      ShaderParamTable::Desc("light_count", ShaderParamType::kInt,
                              offsetof(ps_cbuffer, light_count), 1),
      ShaderParamTable::Desc("lights", offsetof(ps_cbuffer, lights),
                              sizeof(UniverseLight), arraysize(lights_)),
    };
    table = rs->CreateShaderParamTable(arraysize(ps_table_desc), ps_table_desc);
    closure->SetShaderParamTable(kPixelStage, 0, table.get());
    closure->SetShader(shader, 1, 1, 0);
  } else {
    CHECK(false) << "unsupport stage: " << stage;
  }

  return closure;
}

void TextureEffect::SetPV(const Matrix4& value) { pv_ = value;}
void TextureEffect::SetWorld(const Matrix4& value) { world_ = value;}
void TextureEffect::SetCameraPos(const Vector4& pos) { camerapos_ = pos;}
void TextureEffect::SetMaterial(const TextureMaterialData& mtrl) {
  mtrl_ = mtrl;
  SaveShaderResource(kPixelStage, 0, mtrl_.diffusemap.get());
  SaveShaderResource(kPixelStage, 1, mtrl_.specularmap.get());
  SaveShaderResource(kPixelStage, 2, mtrl_.emissionmap.get());
  SaveShaderResource(kPixelStage, 3, mtrl_.alphamap.get());
  SaveShaderResource(kPixelStage, 4, mtrl_.normalmap.get());
}

void TextureEffect::SetLights(const LightPtr* value, int32_t count) {
  DCHECK_LT(count, static_cast<int>(sizeof(lights_)));
  light_count_ = count;
  for (int32_t i = 0; i < count; ++i) {
    Light* light = (value + i)->get();
    memcpy(lights_ + i, &light->data(), sizeof(UniverseLight));
  }
}

void TextureEffect::SetLightData(const UniverseLight* value, int32_t count) {
  memcpy(lights_, value, sizeof(UniverseLight) * std::min(count, kMaxLightCount));
  light_count_ = count;
}

void TextureEffect::ApplyShaderParamTable(Renderer* renderer) {
  {
    ShaderParamTable* tb = GetShaderClosure(kVertexStage)->table_at(0);
    DCHECK(tb != NULL);
    tb->SetValue(0, &pv_, sizeof(Matrix4));
    tb->SetValue(1, &world_, sizeof(Matrix4));
    tb->SetValue(2, &camerapos_, sizeof(Vector4));
  }
  {
    ShaderParamTable* tb = GetShaderClosure(kPixelStage)->table_at(0);
    DCHECK(tb != NULL);
    tb->SetValue(0, &mtrl_.ambient_scalar, sizeof(float));
    tb->SetValue(1, &mtrl_.specular_scalar, sizeof(float));
    tb->SetValue(2, &mtrl_.alpha, sizeof(float));
    tb->SetValue(3, &light_count_, sizeof(light_count_));
    tb->SetValue(4, lights_, sizeof(lights_));
  }
}

}  // namespace azer
