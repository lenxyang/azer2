#include "azer/render/util/camera_object.h"

#include "base/logging.h"
#include "azer/render/blending.h"
#include "azer/render/render_system.h"
#include "azer/render/util/geometry/square_trustum.h"

namespace azer {
CameraObject::CameraObject(const Camera* camera) 
    : camera_(camera) {
  RenderSystem* rs = RenderSystem::Current();
  axes_.reset(new AxesFrames);
  ColoredDiffuseEffectPtr effect = axes_->GetEffect();
  arrow_.reset(new ArrowObject(effect));
  arrow_->SetColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
  
  sphere_color_ = Vector4(1.0f, 1.0f, 1.0f, 0.2f);
  sphere_ = new SphereObject(effect->GetVertexDesc(), 16, 16);

  frustrum_color_ = Vector4(0.0f, 0.255f, 0.761f, 0.6f);
  frustrum_object_ = new SquareTrustum(effect->GetVertexDesc(), 0.4f, 0.1f, 1.0f);

  Blending::Desc blend_desc;
  blend_desc.src = Blending::kSrcAlpha;
  blend_desc.dest = Blending::kSrcInvAlpha;
  blend_desc.oper = Blending::kAdd;
  blend_desc.src_alpha = Blending::kOne;
  blend_desc.dest_alpha = Blending::kZero;
  blend_desc.alpha_oper = Blending::kAdd;
  blend_desc.mask = Blending::kWriteColor;
  blending_ = rs->CreateBlending(blend_desc);
  CHECK(blending_.get());
}

CameraObject::~CameraObject() {
}

void CameraObject::Update(const Camera& camera) {
  world_ = std::move(holder_.GenWorldMatrix());
  pvw_ = std::move(camera.GetProjViewMatrix() * world_);
}

void CameraObject::Render(Renderer* renderer) {
  const TransformHolder* camera_holder = camera_->GetTransformHolder();
  TransformHolder holder = *camera_holder;
  holder.pitch(Degree(-90.0));
  Matrix4 rotate = std::move(holder.orientation().ToMatrix());
  axes_->Render(world_, pvw_, renderer);
  arrow_->Render(world_ * rotate, pvw_ * rotate, renderer);

  Matrix4 frustrum_tran = std::move(Translate(0.0f, 0.5f, 0.0f));
  Matrix4 frustrum_world = std::move(rotate * frustrum_tran);
  renderer->UseBlending(blending_.get(), 0);
  ColoredDiffuseEffectPtr effect = axes_->GetEffect();
  effect->Use(renderer);
  effect->SetDirLight(axes_->light());
  effect->SetColor(frustrum_color_);
  effect->SetWorld(world_ * frustrum_world);
  effect->SetPVW(pvw_ * frustrum_world);
  effect->Use(renderer);
  frustrum_object_->Render(renderer);

  effect->SetColor(sphere_color_);
  effect->SetWorld(world_);
  effect->SetPVW(pvw_);
  effect->Use(renderer);
  sphere_->Render(renderer);
  renderer->ResetBlending();
}
}  // namespace azer
