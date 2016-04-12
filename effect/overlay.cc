#include "azer/effect/overlay.h"

#include "base/lazy_instance.h"
#include "azer/effect/overlay_effect.h"
#include "azer/effect/effectlib.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"

namespace azer {
namespace {
class DefaultBlending {
 public:
  DefaultBlending() {
    Blending::BlendDesc blend_desc;
    blend_desc.desc[0].src = Blending::kSrcAlpha;
    blend_desc.desc[0].dest = Blending::kSrcInvAlpha;
    blend_desc.desc[0].oper = Blending::kAdd;
    blend_desc.desc[0].src_alpha = Blending::kOne;
    blend_desc.desc[0].dest_alpha = Blending::kZero;
    blend_desc.desc[0].alpha_oper = Blending::kAdd;
    blend_desc.desc[0].mask = Blending::kWriteColor;
    blend_desc.desc[0].enable = true;
    blending_ = RenderSystem::Current()->CreateBlending(blend_desc);
    CHECK(blending_.get());
  }
  Blending* blending() { return blending_.get();}
 private:
  BlendingPtr blending_;
  DISALLOW_COPY_AND_ASSIGN(DefaultBlending);
};
::base::LazyInstance<DefaultBlending> g_blending_instance = LAZY_INSTANCE_INITIALIZER;
}

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
  state_ = rs->CreateRasterizerState();
  state_->SetCullingMode(kCullNone);
  blending_ = g_blending_instance.Pointer()->blending();
  bounds_ = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
  texbounds_ = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
}

Overlay::~Overlay() {}

void Overlay::Render(Renderer* renderer) {
  ScopedResetBlending scoped_blending(renderer);
  if (blending_) {
    renderer->SetBlending(blending_, 0, 0xffffffff);
  }

  effect_->SetBounds(bounds_);
  effect_->SetTexBounds(texbounds_);
  effect_->SetTexture(texture_);
  renderer->BindEffect(effect_);
  Draw(renderer);
}

void Overlay::Draw(Renderer* renderer) {
  renderer->BindVertexBuffer(vb_.get());
  renderer->SetPrimitiveTopology(kTriangleStrip);
  renderer->Draw(4, 0);
}

void Overlay::SetTexture(Texture* tex) { 
  if (tex->options().sample_desc.count > 1) {
    effect_ = (OverlayEffect*)EffectLib::instance()->GetEffect("MSOverlayEffect");
  } else {
    effect_ = (OverlayEffect*)EffectLib::instance()->GetEffect("OverlayEffect");
  }
  texture_ = tex;
}
void Overlay::SetBounds(const gfx::RectF& rect) {
  bounds_ = Vector4(rect.x(), rect.y(), rect.width(), rect.height());
}

void Overlay::SetTexBounds(const gfx::RectF& rect) {
  texbounds_ = Vector4(rect.x(), rect.y(), rect.width(), rect.height());
}

Effect* Overlay::effect() { return effect_.get();}
}  // namespace azer
