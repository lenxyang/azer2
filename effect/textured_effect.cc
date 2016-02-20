#include "samples/base/textured_effect.h"

#include "base/strings/utf_string_conversions.h"
#include "lordaeron/env.h"

namespace azer {
using base::UTF8ToUTF16;

IMPLEMENT_EFFECT_DYNCREATE(TexturedEffect);
const char TexturedEffect::kEffectName[] = "TexturedEffect";
TexturedEffect::TexturedEffect()
    : ambient_scalar_(0.01f),
      specular_scalar_(1.0f),
      alpha_(1.0f) {
}

TexturedEffect::~TexturedEffect() {
}

const char* TexturedEffect::GetEffectName() const {
  return kEffectName;
}

void TexturedEffect::InitGpuConstantTable() {
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
    GpuConstantsTable::Desc("pad2", GpuConstantsType::kFloat,
                            offsetof(ps_cbuffer, pad2), 1),
    GpuConstantsTable::Desc("lights", offsetof(ps_cbuffer, lights),
                            sizeof(UniverseLight), 4),
  };
  gpu_table_[kPixelStage] = rs->CreateGpuConstantsTable(
      arraysize(ps_table_desc), ps_table_desc);
}

void TexturedEffect::SetPV(const Matrix4& value) {
  pv_ = value;
}
void TexturedEffect::SetWorld(const Matrix4& value) {
  world_ = value;
}
void TexturedEffect::SetCameraPos(const Vector4& value) {
  camerapos_ = value;
}

void TexturedEffect::SetLights(const LightPtr* value, int32 count) {
  DCHECK_LT(count, sizeof(lights_));
  for (int32 i = 0; i < count; ++i) {
    Light* light = (value + i)->get();
    memcpy(lights_ + i, &light->data(), sizeof(UniverseLight));
  }
}

void TexturedEffect::ApplyGpuConstantTable(Renderer* renderer) {
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
    tb->SetValue(0, &ambient_scalar_, sizeof(float));
    tb->SetValue(1, &specular_scalar_, sizeof(float));
    tb->SetValue(2, &alpha_, sizeof(float));
    tb->SetValue(3, &specular_scalar_, sizeof(float));
    tb->SetValue(4, lights_, sizeof(lights_));
  }
}

void TexturedEffect::UseTexture(Renderer* renderer) {
  renderer->BindTexture(kPixelStage, 0, diffuse_map_.get());
}

}  // namespace azer
