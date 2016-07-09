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

void AmbientColorEffect::InitGpuConstantTable() {
  RenderSystem* rs = RenderSystem::Current();
  // generate GpuTable init for stage kVertexStage
  GpuConstantsTable::Desc vs_table_desc[] = {
    GpuConstantsTable::Desc("pv", GpuConstantsType::kMatrix4,
                            offsetof(vs_cbuffer, pv), 1),
    GpuConstantsTable::Desc("world", GpuConstantsType::kMatrix4,
                            offsetof(vs_cbuffer, world), 1),
    GpuConstantsTable::Desc("camerapos", GpuConstantsType::kVector4,
                            offsetof(vs_cbuffer, camerapos), 1),
  };

  GpuVariable v;
  v.table = rs->CreateGpuConstantsTable(arraysize(vs_table_desc), vs_table_desc);
  v.stage = kVertexStage;
  v.type = kUpdatePerFrame;
  gpu_table_.push_back(v);
  
  // generate GpuTable init for stage kPixelStage
  GpuConstantsTable::Desc ps_table_desc[] = {
    GpuConstantsTable::Desc("ambient", offsetof(ps_cbuffer, ambient),
                            sizeof(Vector4), 1),
  };

  v.table = rs->CreateGpuConstantsTable(arraysize(ps_table_desc), ps_table_desc);
  v.stage = kPixelStage;
  v.type = kUpdatePerFrame;
  gpu_table_.push_back(v);
}

void AmbientColorEffect::ApplyGpuConstantTable(Renderer* renderer) {
  {
    GpuVariable gv = gpu_table_[0];
    CHECK_EQ(gv.stage, kVertexStage);
    GpuConstantsTable* tb = gv.table.get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &pv_, sizeof(Matrix4));
    tb->SetValue(1, &world_, sizeof(Matrix4));
  }
  {
    GpuVariable gv = gpu_table_[1];
    CHECK_EQ(gv.stage, kPixelStage);
    GpuConstantsTable* tb = gv.table.get();
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

void ColorEffect::InitGpuConstantTable() {
  RenderSystem* rs = RenderSystem::Current();
  // generate GpuTable init for stage kVertexStage
  GpuConstantsTable::Desc vs_table_desc[] = {
    GpuConstantsTable::Desc("pv", GpuConstantsType::kMatrix4,
                            offsetof(vs_cbuffer, pv), 1),
    GpuConstantsTable::Desc("world", GpuConstantsType::kMatrix4,
                            offsetof(vs_cbuffer, world), 1),
    GpuConstantsTable::Desc("camerapos", GpuConstantsType::kVector4,
                            offsetof(vs_cbuffer, camerapos), 1),
  };
  GpuVariable v;
  v.table = rs->CreateGpuConstantsTable(arraysize(vs_table_desc), vs_table_desc);
  v.stage = kVertexStage;
  v.type = kUpdatePerFrame;
  gpu_table_.push_back(v);
  // generate GpuTable init for stage kPixelStage
  GpuConstantsTable::Desc ps_table_desc[] = {
    GpuConstantsTable::Desc("mtrl", offsetof(ps_cbuffer, mtrl),
                            sizeof(ColorMaterialData), 1),
    GpuConstantsTable::Desc("lights", offsetof(ps_cbuffer, lights),
                            sizeof(UniverseLight), 4),
    GpuConstantsTable::Desc("light_count", GpuConstantsType::kInt,
                            offsetof(ps_cbuffer, light_count), 1),
  };
  v.table = rs->CreateGpuConstantsTable(arraysize(ps_table_desc), ps_table_desc);
  v.stage = kPixelStage;
  v.type = kUpdatePerFrame;
  gpu_table_.push_back(v);
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
    GpuVariable gv = gpu_table_[0];
    CHECK_EQ(gv.stage, kVertexStage);
    GpuConstantsTable* tb = gv.table.get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &pv_, sizeof(Matrix4));
    tb->SetValue(1, &world_, sizeof(Matrix4));
    tb->SetValue(2, &camerapos_, sizeof(Vector4));
  }
  {
    GpuVariable gv = gpu_table_[1];
    CHECK_EQ(gv.stage, kPixelStage);
    GpuConstantsTable* tb = gv.table.get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &mtrl_, sizeof(mtrl_));
    tb->SetValue(1, lights_, sizeof(UniverseLight) * 4);
    tb->SetValue(2, &light_count_, sizeof(light_count_));
  }
}
}  // namespace azer
