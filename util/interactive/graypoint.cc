#include "azer/util/interactive/graypoint.h"

#include "azer/effect/effectlib.h"
#include "azer/effect/text_billboard_effect.h"
#include "azer/render/camera.h"
#include "azer/render/render_system.h"
#include "azer/render/texture_view.h"
#include "azer/render/vertex_pack.h"
#include "azer/res/grit/common.h"
#include "azer/util/geometry/arrow.h"
#include "azer/util/interactive/env.h"
#include "azer/util/interactive/orientation_util.h"

namespace azer {
GrayPoint::GrayPoint() 
    : scale_(1.0f, 1.0f, 1.0f) {
  color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
  scale_ = Vector3(1.0f, 1.0f, 1.0f);
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  effect_ = (TextBillboardEffect*)env->GetEffect("TextBillboardEffect");
  EntityDataPtr ptr(new EntityData(effect_->vertex_desc(), 1));
  VertexPack pack(ptr->vdata());
  pack.first();
  pack.WriteVector3Or4(Vector4(0.0f, 0.0f, 0.0f, 1.0f), VertexPos(0, 0));
  ptr->AddSubset(Subset(0, 1, 0, 0, kPointList));
  point_ = new Entity(ptr.get());
  Texture* tex = env->GetTexture(IMG_POINTGRAY);
  RenderSystem* rs = RenderSystem::Current();
  pointgray_texview_ = rs->CreateTextureView(TextureView::Options(), tex);

  effect_->SetTexture(pointgray_texview_.get());
}

GrayPoint::~GrayPoint() {
}

void GrayPoint::Render(const Camera& camera, Renderer* renderer) {
  Matrix4 world = std::move(Transform::Translate(position_));
  effect_->SetWorld(world);
  effect_->SetPV(camera.GetProjViewMatrix());
  effect_->SetDiffuse(color_);
  effect_->SetViewPos(camera.position());
  effect_->SetViewUp(camera.up());
  effect_->SetBillboard(scale_.x, scale_.y);
  renderer->BindEffect(effect_.get());
  point_->Draw(renderer);
}
}  // namespace azer
