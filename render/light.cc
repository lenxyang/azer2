#include "azer/render/light.h"

#include "base/logging.h"
#include "azer/render/render_target.h"
#include "azer/render/texture.h"

namespace azer {
Light::Light(int32 id) {
}

RendererPtr Light::GetShadowRenderer() {
  DCHECK(renderer_.get());
  return renderer_;
}

TexturePtr Light::GetShadowMap() {
  DCHECK(renderer_.get() && renderer_->GetRenderTarget().get());
  return renderer_->GetRenderTarget()->GetTexture();
}

void Light::OnPositionChanged(const Vector3& origin_position) {
  camera_.GetTransformHolder()->SetPosition(position());
  camera_.GetTransformHolder()->set_orientation(orientation());
}

void Light::OnOrientationChanged(const Quaternion& origin_orientation) {
  camera_.GetTransformHolder()->SetPosition(position());
  camera_.GetTransformHolder()->set_orientation(orientation());
}
}  // namespace azer
