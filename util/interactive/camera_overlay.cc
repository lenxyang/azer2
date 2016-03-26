#include "azer/util/interactive/camera_overlay.h"

#include "azer/math/math.h"
#include "azer/effect/overlay.h"
#include "azer/render/camera.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"
#include "azer/render/texture.h"
#include "azer/render/transform_holder.h"
#include "azer/util/interactive/axes_frame.h"
#include "azer/util/interactive/env.h"

namespace azer {
CameraOverlay::CameraOverlay(const Camera* camera) 
    : camera_(camera) {
  RenderSystem* rs = RenderSystem::Current();
  Texture::Options rdopt, depthopt;
  rdopt.size = gfx::Size(128, 128);
  rdopt.target = (kBindTargetRenderTarget | kBindTargetShaderResource);
  depthopt.size = rdopt.size;
  depthopt.target = kBindTargetDepthStencil;
  renderer_ = rs->CreateRenderer(rdopt, depthopt);
  viewport_.bounds = gfx::Rect(rdopt.size);
  renderer_->SetViewport(viewport_);

  overlay_ = new Overlay;
  overlay_->SetBounds(gfx::RectF(0.75f, 0.75f, 0.25f, 0.25f));
  overlay_->SetTexBounds(gfx::RectF(0.0f, 0.0f, 1.0f, 1.0f));
  object_.reset(new AxesFrame);
}

CameraOverlay::~CameraOverlay() {}

void CameraOverlay::SetBounds(const gfx::RectF& bounds) {
  overlay_->SetBounds(bounds);
}

void CameraOverlay::Update() {
  const TransformHolder& holder = camera_->holder();
  
  Vector3 position(-holder.direction() * 2.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(holder.up());
  Camera camera;
  camera.reset(position, lookat, up);
  camera.mutable_frustum()->set_aspect(1.0f);
  world_ = Matrix4::kIdentity;

  Renderer* texrd = renderer_.get();
  texrd->Use();
  texrd->Clear(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
  texrd->ClearDepthAndStencil();
  object_->Update(&camera);
  object_->Render(texrd);
}

void CameraOverlay::Render(Renderer* renderer) {
  InteractiveEnv* env = InteractiveEnv::GetInstance();
  Blending* blending = env->blending();
  renderer->SetBlending(blending, 0, 0xffffffff);
  overlay_->SetTexture(renderer_->GetRenderTarget(0)->GetTexture());
  overlay_->Render(renderer);
  renderer->ResetBlending();
}
}  // namespace azer