#include "azer/effect/color_effect.h"

#include "base/strings/utf_string_conversions.h"
#include "azer/azer.h"

namespace azer {
using base::UTF8ToUTF16;

IMPLEMENT_EFFECT_DYNCREATE(AmbientColorEffect);
const char AmbientColorEffect::kEffectName[] = "azer::AmbientColorEffect";
AmbientColorEffect::AmbientColorEffect() {}
AmbientColorEffect::~AmbientColorEffect() {}
const char* AmbientColorEffect::GetEffectName() const { return kEffectName;}

ShaderClosurePtr AmbientColorEffect::InitShaderClosure(RenderPipelineStage stage,
                                                       Shader* shader) {
  RenderSystem* rs = RenderSystem::Current();
  GpuConstantsTablePtr table;
  ShaderClosurePtr closure(new ShaderClosure(stage));
  if (stage == kPixelStage) {
    // generate GpuTable init for stage kVertexStage
    GpuConstantsTable::Desc vs_table_desc[] = {
      GpuConstantsTable::Desc("pv", GpuConstantsType::kMatrix4,
                              offsetof(vs_cbuffer, pv), 1),
      GpuConstantsTable::Desc("world", GpuConstantsType::kMatrix4,
                              offsetof(vs_cbuffer, world), 1),
      GpuConstantsTable::Desc("camerapos", GpuConstantsType::kVector4,
                              offsetof(vs_cbuffer, camerapos), 1),
    };

    
    closure->SetShader(shader, 1, 0, 0);
    table = rs->CreateGpuConstantsTable(arraysize(vs_table_desc), vs_table_desc);
    closure->SetGpuConstantsTable(0, 1, table.get());
  } else if (stage == kPixelStage) {
    // generate GpuTable init for stage kPixelStage
    GpuConstantsTable::Desc ps_table_desc[] = {
      GpuConstantsTable::Desc("ambient", offsetof(ps_cbuffer, ambient),
                              sizeof(Vector4), 1),
    };

    table = rs->CreateGpuConstantsTable(arraysize(ps_table_desc), ps_table_desc);
    closure->SetShader(shader, 1, 0, 0);
    closure->SetGpuConstantsTable(stage, 0, table.get());
  } else {
    CHECK(false) << "not support stage: " << stage;
  }

  return closure;
}

void AmbientColorEffect::ApplyGpuConstantTable(Renderer* renderer) {
  {
    GpuConstantsTable* tb = GetShaderClosure(kVertexStage)->table_at(0);
    DCHECK(tb != NULL);
    tb->SetValue(0, &pv_, sizeof(Matrix4));
    tb->SetValue(1, &world_, sizeof(Matrix4));
  }
  {
    GpuConstantsTable* tb = GetShaderClosure(kPixelStage)->table_at(0);
    DCHECK(tb != NULL);
    tb->SetValue(0, &ambient_, sizeof(Vector4));
  }
}

// class ColorEffect
IMPLEMENT_EFFECT_DYNCREATE(ColorEffect);
const char ColorEffect::kEffectName[] = "azer::ColorEffect";
ColorEffect::ColorEffect() : light_count_(0) {}
ColorEffect::~ColorEffect() {}
const char* ColorEffect::GetEffectName() const { return kEffectName;}

ShaderClosurePtr ColorEffect::InitShaderClosure(RenderPipelineStage stage,
                                                Shader* shader) {
  RenderSystem* rs = RenderSystem::Current();
  ShaderClosurePtr closure(new ShaderClosure(stage));
  GpuConstantsTablePtr table;
  if (stage == kVertexStage) {
    // generate GpuTable init for stage kVertexStage
    GpuConstantsTable::Desc vs_table_desc[] = {
      GpuConstantsTable::Desc("pv", GpuConstantsType::kMatrix4,
                              offsetof(vs_cbuffer, pv), 1),
      GpuConstantsTable::Desc("world", GpuConstantsType::kMatrix4,
                              offsetof(vs_cbuffer, world), 1),
      GpuConstantsTable::Desc("camerapos", GpuConstantsType::kVector4,
                              offsetof(vs_cbuffer, camerapos), 1),
    };
    
    table = rs->CreateGpuConstantsTable(arraysize(vs_table_desc), vs_table_desc);
    closure->SetGpuConstantsTable(kVertexStage, 0, table.get());
    closure->SetShader(shader, 1, 0, 0);
  } else if (stage == kPixelStage) {
    // generate GpuTable init for stage kPixelStage
    GpuConstantsTable::Desc ps_table_desc[] = {
      GpuConstantsTable::Desc("mtrl", offsetof(ps_cbuffer, mtrl),
                              sizeof(ColorMaterialData), 1),
      GpuConstantsTable::Desc("lights", offsetof(ps_cbuffer, lights),
                              sizeof(UniverseLight), 4),
      GpuConstantsTable::Desc("light_count", GpuConstantsType::kInt,
                              offsetof(ps_cbuffer, light_count), 1),
    };
    table = rs->CreateGpuConstantsTable(arraysize(ps_table_desc), ps_table_desc);
    SetGpuConstantsTable(kPixelStage, 0, table.get());
    closure->SetShader(shader, 1, 0, 0);
  } else {
    CHECK(false);
  }

  return closure;
}

void ColorEffect::SetPV(const Matrix4& value) { pv_ = value;}
void ColorEffect::SetWorld(const Matrix4& value) { world_ = value;}
void ColorEffect::SetCameraPos(const Vector4& pos) { camerapos_ = pos;}
void ColorEffect::SetMaterial(const ColorMaterialData& mtrl) {
  mtrl_ = mtrl;
}

void ColorEffect::SetLights(const LightPtr* value, int32_t count) {
  DCHECK_LT(count, static_cast<int>(sizeof(lights_)));
  light_count_ = count;
  for (int32_t i = 0; i < count; ++i) {
    Light* light = (value + i)->get();
    memcpy(lights_ + i, &light->data(), sizeof(UniverseLight));
  }
}

void ColorEffect::SetLightData(const UniverseLight* value, int32_t count) {
  memcpy(lights_, value, sizeof(UniverseLight) * std::min(count, kMaxLightCount));
  light_count_ = count;
}

void ColorEffect::ApplyGpuConstantTable(Renderer* renderer) {
  {
    GpuConstantsTable* tb = GetShaderClosure(kVertexStage)->table_at(0);
    DCHECK(tb != NULL);
    tb->SetValue(0, &pv_, sizeof(Matrix4));
    tb->SetValue(1, &world_, sizeof(Matrix4));
    tb->SetValue(2, &camerapos_, sizeof(Vector4));
  }
  {
    GpuConstantsTable* tb = GetShaderClosure(kPixelStage)->table_at(0);
    DCHECK(tb != NULL);
    tb->SetValue(0, &mtrl_, sizeof(mtrl_));
    tb->SetValue(1, lights_, sizeof(UniverseLight) * 4);
    tb->SetValue(2, &light_count_, sizeof(light_count_));
  }
}
}  // namespace azer
