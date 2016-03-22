#include "azer/effect/overlay.h"

#include "azer/effect/overlay_effect.h"
#include "azer/effect/effectlib.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"

namespace azer {
Overlay::Overlay(EffectLib* lib) {
  effect_ = Effectlib::instance()->GetEffect("OverlayEffect");
  SlotVertexData vdata(effect_->vertex_desc(), 4);
  vb_ = rs->CreateVertexBuffer(kVertexBufferOpt, vdata);
}

Overlay::~Overlay() {}

void Overlay::Render(Renderer* renderer) {
  ScopedResetBlending scoped_blending(renderer);
  renderer->SetBlending(blending_, 0, 0xffffffff);
  renderer->BindVertexBufferGroup(vbg_.get());
  renderer->SetPrimitiveTopology(kTriangleStrip);
  renderer->Draw(4, 0);
}

void Overlay::SetTexture(Texture* tex) { effect_->SetTexture(tex);}

void Overlay::SetBounds(const gfx::RectF& rect) {
  Vector4 vec(rect.left(), rect.top(), rect.width(), rect.height());
  effect_->SetParams(vec);
}
}  // namespace azer
