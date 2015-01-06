#include "azer/render/overlay.h"
#include "azer/render/render.h"

namespace azer {

void OverlayEffect::Use(Renderer* renderer) {
  DCHECK(texture_ptr_.get() != NULL);
  renderer->UseTexture(azer::kPixelStage, 0, texture_ptr_.get());
  UseConstantsTable(renderer);
  UseTechnique(renderer);
}

Overlay::Overlay(const gfx::RectF& rect) 
    : blending_enabled_(true)
    , rect_(rect)
    , texcoord1_(0.0f, 0.0f)
    , texcoord2_(1.0f, 1.0f) {
}

Overlay::~Overlay() {
}

bool Overlay::Init(RenderSystem* rs) {
  Blending::Desc blend_desc;
  blend_desc.src = Blending::kSrcAlpha;
  blend_desc.dest = Blending::kSrcInvAlpha;
  blend_desc.oper = Blending::kAdd;
  blend_desc.src_alpha = Blending::kOne;
  blend_desc.dest_alpha = Blending::kZero;
  blend_desc.alpha_oper = Blending::kAdd;
  blend_desc.mask = Blending::kWriteColor;
  blending_.reset(rs->CreateBlending(blend_desc));
  if (!blending_.get()) {
    LOG(ERROR) << "Failed to create blending.";
    return false;
  }

  return true;
}

void Overlay::SetTexture(TexturePtr tex) {
  OverlayEffect* effect = (OverlayEffect*)effect_ptr_.get();
  effect->SetTexture(tex);
}

void Overlay::SetBlending(Renderer* renderer) {
  DCHECK(NULL != blending_.get());
  if (IsBlendingEnable()) {
    renderer->UseBlending(blending_.get(), 0);
  }
}

void Overlay::ResetBlending(Renderer* renderer) {
  DCHECK(NULL != blending_.get());
  if (IsBlendingEnable()) {
    renderer->ResetBlending();
  }
}

void Overlay::Render(Renderer* renderer) {
  SetBlending(renderer);
  DCHECK (effect_ptr_.get() != NULL);
  effect_ptr_->Use(renderer);
  renderer->Draw(vb_ptr_.get(), azer::kTriangleList, 6);
  ResetBlending(renderer);
}
}  // namespace azer
