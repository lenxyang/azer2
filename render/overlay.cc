#include "azer/render/overlay.h"
#include "azer/render/render.h"

namespace azer {
Overlay::Overlay() {
  vertex_[0] = azer::Vector4(-1.0f,  1.0f, 0.0f, 1.0f);
  vertex_[1] = azer::Vector4( 1.0f,  1.0f, 0.0f, 1.0f);
  vertex_[2] = azer::Vector4( 1.0f, -1.0f, 0.0f, 1.0f);
  vertex_[3] = azer::Vector4(-1.0f, -1.0f, 0.0f, 1.0f);

  texcoord_[0] = azer::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
  texcoord_[1] = azer::Vector4(1.0f, 0.0f, 0.0f, 0.0f);
  texcoord_[2] = azer::Vector4(1.0f, 1.0f, 0.0f, 0.0f);
  texcoord_[3] = azer::Vector4(0.0f, 1.0f, 0.0f, 0.0f);
  transform_ = Matrix4::kIdentity;
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

void Overlay::SetTexCoord(const gfx::PointF& lt, const gfx::PointF& br) {
  texcoord_[0] = azer::Vector4(lt.x(), lt.y(), 0.0f, 0.0f);
  texcoord_[1] = azer::Vector4(br.x(), lt.y(), 0.0f, 0.0f);
  texcoord_[2] = azer::Vector4(br.x(), br.y(), 0.0f, 0.0f);
  texcoord_[3] = azer::Vector4(lt.x(), br.y(), 0.0f, 0.0f);
}

void Overlay::SetTexCoord(const gfx::RectF& rc) {
  SetTexCoord(gfx::PointF(rc.x(), rc.y()),
              gfx::PointF(rc.x() + rc.width(), rc.y() + rc.height()));
}

void Overlay::SetBounds(const gfx::RectF& rect) {
  vertex_[0] = azer::Vector4(rect.x(), rect.y() + rect.height(), 0.0f, 1.0f);
  vertex_[1] = azer::Vector4(rect.x() + rect.width(), rect.y() + rect.height(),
                             0.0f, 1.0f);
  vertex_[2] = azer::Vector4(rect.x() + rect.width(), rect.y(), 0.0f, 1.0f);
  vertex_[3] = azer::Vector4(rect.x(), rect.y(), 0.0f, 1.0f);
}
}  // namespace azer
