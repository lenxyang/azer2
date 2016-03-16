#include "azer/util/interactive/axes_frame.h"

#include "base/logging.h"
#include "azer/effect/effect.h"
#include "azer/effect/color_effect.h"
#include "azer/effect/text_billboard_effect.h"
#include "azer/render/camera.h"
#include "azer/render/entity.h"
#include "azer/render/renderer.h"
#include "azer/render/vertex_pack.h"
#include "azer/util/interactive/env.h"
#include "azer/res/grit/common.h" 

namespace azer {
AxesFrame::AxesFrame() {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  effect_ = (AmbientColorEffect*)env->GetEffect("AmbientColorEffect");
  texeffect_ = (TextBillboardEffect*)env->GetEffect("TextBillboardEffect");
  scale_ = Vector3(1.0f, 1.0f, 1.0f);
  scoped_refptr<EntityData> data(new EntityData(effect_->vertex_desc(), 9));
  VertexPack vpack(data->vdata());
  vpack.first();
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(1.0f, 0.0f, 0.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 1.0f, 0.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 1.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));

  // 
  vpack.WriteVector3Or4(Vector4(1.1f, 0.0f, 0.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 1.1f, 0.0f, 1.0f), VertexPos(0, 0));
  CHECK(vpack.next(1));
  vpack.WriteVector3Or4(Vector4(0.0f, 0.0f, 1.1f, 1.0f), VertexPos(0, 0));

  entity_ = new Entity(data);
  entity_->AddSubset(Subset(0, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(2, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(4, 2, 0, 0, kLineList));
  entity_->AddSubset(Subset(6, 1, 0, 0, kPointList));
  entity_->AddSubset(Subset(7, 1, 0, 0, kPointList));
  entity_->AddSubset(Subset(8, 1, 0, 0, kPointList));

  colors_[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  colors_[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
  colors_[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

  tex_[0] =  env->GetTexture(IMG_COORD_X);
  tex_[1] =  env->GetTexture(IMG_COORD_Y);
  tex_[2] =  env->GetTexture(IMG_COORD_Z);
}

AxesFrame::~AxesFrame() {}

void AxesFrame::Update(const Camera* camera, const Vector3& position) {
  effect_->SetPV(camera->GetProjViewMatrix());
  Matrix4 mat = std::move(Scale(scale_));
  mat = std::move(std::move(Translate(position)) * mat);
  effect_->SetWorld(mat);
  texeffect_->SetPV(camera->GetProjViewMatrix());
  texeffect_->SetWorld(Matrix4::kIdentity);
  texeffect_->SetViewPos(camera->position());
  texeffect_->SetViewUp(camera->up());
  texeffect_->SetBillboard(0.05f * scale_.x, 0.05f * scale_.y);
}

void AxesFrame::SetColor(int32 index, const Vector4& color) {
  DCHECK_LT(index, static_cast<int32>(arraysize(colors_)));
  colors_[index] = color;
}

void AxesFrame::Render(Renderer* renderer) {
  for (uint32 i = 0; i < arraysize(colors_); ++i) {
    effect_->SetAmbient(colors_[i]);
    renderer->BindEffect(effect_);
    entity_->DrawSub(i, renderer);
  }

  for (uint32 i = 0; i < arraysize(colors_); ++i) {
    texeffect_->SetDiffuse(colors_[i]);
    texeffect_->SetTexture(tex_[i]);
    renderer->BindEffect(texeffect_);
    entity_->DrawSub(i + 3, renderer);
  }
}
}  // namespace azer
