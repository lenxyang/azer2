#include "azer/effect/overlay.h"

#include "azer/effect/overlay_effect.h"
#include "azer/effect/effectlib.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"

namespace azer {
Overlay::Overlay() {
  RenderSystem* rs = RenderSystem::Current();
  effect_ = (OverlayEffect*)EffectLib::instance()->GetEffect("OverlayEffect");
  SlotVertexDataPtr vdata(new SlotVertexData(effect_->vertex_desc(), 4));
  vb_ = rs->CreateVertexBuffer(kVertexBufferOpt(), vdata);
}

Overlay::~Overlay() {}

void Overlay::Render(Renderer* renderer) {
  ScopedResetBlending scoped_blending(renderer);
  renderer->SetBlending(blending_, 0, 0xffffffff);
  renderer->BindVertexBuffer(vb_.get());
  renderer->SetPrimitiveTopology(kTriangleStrip);
  renderer->Draw(4, 0);
}

void Overlay::SetTexture(Texture* tex) { effect_->SetTexture(tex);}

void Overlay::SetBounds(const gfx::RectF& rect) {
  Vector4 vec(rect.x(), rect.y(), rect.width(), rect.height());
  effect_->SetBounds(vec);
}

void Overlay::SetTexBounds(const gfx::RectF& rect) {
  Vector4 vec(rect.x(), rect.y(), rect.width(), rect.height());
  effect_->SetTexBounds(vec);
}
}  // namespace azer
