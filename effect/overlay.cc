#include "azer/effect/overlay.h"

#include "azer/effect/overlay_effect.h"
#include "azer/effect/effectlib.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"

namespace azer {
Overlay::Overlay() {
  Vector4 position[] = {
    {-1.0f,  1.0f, 0.0f, 1.0f},
    {-1.0f, -1.0f, 0.0f, 1.0f},
    { 1.0f,  1.0f, 0.0f, 1.0f},
    { 1.0f, -1.0f, 0.0f, 1.0f},
  };
  Vector2 texcoord[] = {
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
  };
  RenderSystem* rs = RenderSystem::Current();
  effect_ = (OverlayEffect*)EffectLib::instance()->GetEffect("OverlayEffect");
  SlotVertexDataPtr vdata(new SlotVertexData(effect_->vertex_desc(), 4));
  VertexPos vpos(0, 0), tpos;
  VertexPack vpack(vdata);
  GetSemanticIndex("texcoord", 0, effect_->vertex_desc(), &tpos);
  vpack.first();
  for (uint32 i = 0; i < arraysize(position); ++i) { 
    vpack.WriteVector3Or4(position[i], vpos);
    vpack.WriteVector2(texcoord[i], tpos);
    vpack.next(1);
  }

  vb_ = rs->CreateVertexBuffer(kVertexBufferOpt(), vdata);
  rasterizer_state_ = rs->CreateRasterizerState();
  rasterizer_state_->SetCullingMode(kCullNone);
}

Overlay::~Overlay() {}

void Overlay::Render(Renderer* renderer) {
  ScopedResetBlending scoped_blending(renderer);
  ScopedRasterizerState scoped_rasterizer(renderer);
  renderer->SetRasterizerState(state_);
  if (blending_) {
    renderer->SetBlending(blending_, 0, 0xffffffff);
  }

  effect_->SetBounds(bounds_);
  effect_->SetTexBounds(texbounds_);
  effect_->SetTexture(tex);
  renderer->BindEffect(effect_);
  renderer->BindVertexBuffer(vb_.get());
  renderer->SetPrimitiveTopology(kTriangleStrip);
  renderer->Draw(4, 0);
}

void Overlay::SetTexture(Texture* tex) { texture_ = tex;}
void Overlay::SetBounds(const gfx::RectF& rect) {
  bounds = Vector4(rect.x(), rect.y(), rect.width(), rect.height());
}

void Overlay::SetTexBounds(const gfx::RectF& rect) {
  texbounds_ = Vector4(rect.x(), rect.y(), rect.width(), rect.height());
}
}  // namespace azer
