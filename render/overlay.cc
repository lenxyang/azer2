#include "azer/render/overlay.h"
#include "azer/render/render.h"

namespace azer {
Overlay::Overlay(const gfx::RectF& rect) 
    : rect_(rect)
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

void Overlay::SetBlending(Renderer* renderer) {
  if (blending_.get()) {
    renderer->UseBlending(blending_.get(), 0);
  }
}

void Overlay::ResetBlending(Renderer* renderer) {
  if (blending_.get()) {
    renderer->ResetBlending();
  }
}
}  // namespace azer
