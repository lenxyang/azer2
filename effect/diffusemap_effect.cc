#include "azer/effect/diffusemap_effect.h"

#include "base/strings/utf_string_conversions.h"
#include "azer/render/render.h"


namespace azer {
using base::UTF8ToUTF16;

IMPLEMENT_EFFECT_DYNCREATE(DiffuseMapEffect);
const char DiffuseMapEffect::kEffectName[] = "azer::DiffuseMapEffect";
DiffuseMapEffect::DiffuseMapEffect() : light_count_(0) {}
DiffuseMapEffect::~DiffuseMapEffect() {}
const char* DiffuseMapEffect::GetEffectName() const { return kEffectName;}

void DiffuseMapEffect::InitGpuConstantTable() {
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
  gpu_table_[kVertexStage] = rs->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);
  // generate GpuTable init for stage kPixelStage
  GpuConstantsTable::Desc ps_table_desc[] = {
    GpuConstantsTable::Desc("ambient_scalar", GpuConstantsType::kFloat,
                            offsetof(ps_cbuffer, ambient_scalar), 1),
    GpuConstantsTable::Desc("specular_scalar", GpuConstantsType::kFloat,
                            offsetof(ps_cbuffer, specular_scalar), 1),
    GpuConstantsTable::Desc("alpha", GpuConstantsType::kFloat,
                            offsetof(ps_cbuffer, alpha), 1),
    GpuConstantsTable::Desc("light_count", GpuConstantsType::kInt,
                            offsetof(ps_cbuffer, light_count), 1),
    GpuConstantsTable::Desc("lights", offsetof(ps_cbuffer, lights),
                            sizeof(UniverseLight), arraysize(lights_)),
  };
  gpu_table_[kPixelStage] = rs->CreateGpuConstantsTable(
      arraysize(ps_table_desc), ps_table_desc);
}

void DiffuseMapEffect::SetPV(const Matrix4& value) { pv_ = value;}
void DiffuseMapEffect::SetWorld(const Matrix4& value) { world_ = value;}
void DiffuseMapEffect::SetCameraPos(const Vector4& pos) { camerapos_ = pos;}
void DiffuseMapEffect::SetLights(const LightPtr* value, int32 count);
void DiffuseMapEffect::SetLightData(const UniverseLight* value, int32 count);
void DiffuseMapEffect::SetMaterial(const DiffuseMapMaterialData& mtrl) { mtrl_ = mtrl;}

void DiffuseMapEffect::SetLights(const LightPtr* value, int32 count) {
  DCHECK_LT(count, sizeof(lights_));
  light_count_ = count;
  for (int32 i = 0; i < count; ++i) {
    Light* light = (value + i)->get();
    memcpy(lights_ + i, &light->data(), sizeof(UniverseLight));
  }
}

void DiffuseMapEffect::SetLightData(const UniverseLight* value, int32 count) {
  memcpy(lights_, value, sizeof(UniverseLight) * std::min(count, kMaxLightCount));
}

void DiffuseMapEffect::ApplyGpuConstantTable(Renderer* renderer) {
  {
    GpuConstantsTable* tb = gpu_table_[(int)kVertexStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &pv_, sizeof(Matrix4));
    tb->SetValue(1, &world_, sizeof(Matrix4));
    tb->SetValue(2, &camerapos_, sizeof(Vector4));
  }
  {
    GpuConstantsTable* tb = gpu_table_[(int)kPixelStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &mtrl_.ambient_scalar, sizeof(float));
    tb->SetValue(1, &mtrl_.specular_scalar, sizeof(float));
    tb->SetValue(2, &mtrl_.alpha, sizeof(float));
    tb->SetValue(3, &light_count_, sizeof(light_count_));
    tb->SetValue(4, lights_, sizeof(lights_));
  }
}

void DiffuseMapEffect::UseTexture(Renderer* renderer) {
  renderer->BindTexture(kPixelStage, 0, mtrl_.diffusemap.get());
}
}  // namespace azer
