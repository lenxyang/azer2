#include "azer/render/light.h"

#include "base/logging.h"
#include "azer/render/render_target.h"
#include "azer/render/texture.h"

namespace azer {
Light::Light(int32 id) {
}

Renderer* Light::GetShadowRenderer() {
  DCHECK(renderer_.get());
  return renderer_.get();
}

Texture* Light::GetShadowMap() {
  DCHECK(renderer_.get() && renderer_->GetRenderTarget(0));
  return renderer_->GetRenderTarget(0)->GetTexture();
}

void Light::OnPositionChanged(const Vector3& origin_position) {
  camera_.mutable_holder()->SetPosition(position());
  camera_.mutable_holder()->set_orientation(orientation());
}

void Light::OnOrientationChanged(const Quaternion& origin_orientation) {
  camera_.mutable_holder()->SetPosition(position());
  camera_.mutable_holder()->set_orientation(orientation());
}
}  // namespace azer
