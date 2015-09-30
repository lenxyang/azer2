#include "azer/samples/camera/camera_sphere_object.h"

#include "base/logging.h"
#include "azer/render/blending.h"
#include "azer/render/render_system.h"

namespace azer {
CameraSphereObject::CameraSphereObject(const Camera* camera) 
    : camera_(camera) {
  RenderSystem* rs = RenderSystem::Current();
  axes_.reset(new AxesFrames);
  ColoredDiffuseEffectPtr effect = axes_->GetEffect();
  arrow_.reset(new ArrowObject(effect));
  arrow_->SetColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
  
  sphere_ = new SphereObject(effect->GetVertexDesc(), 16, 16);

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

CameraSphereObject::~CameraSphereObject() {
}

void CameraSphereObject::Update() {
  world_ = std::move(holder_.GenWorldMatrix());
  pvw_ = std::move(camera_->GetProjViewMatrix() * world_);
}

void CameraSphereObject::Render(Renderer* renderer) {
  Update();
  
  axes_->Render(world_, pvw_, renderer);
  renderer->UseBlending(blending_.get(), 0);

  ColoredDiffuseEffectPtr effect = axes_->GetEffect();
  effect->Use(renderer);
  effect->SetDirLight(axes_->light());
  effect->SetColor(Vector4(1.0f, 1.0f, 1.0f, 0.3f));
  effect->SetWorld(world_);
  effect->SetPVW(pvw_);
  effect->Use(renderer);
  sphere_->Render(renderer);
  renderer->ResetBlending();
}
}  // namespace azer
