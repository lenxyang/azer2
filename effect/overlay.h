#pragma once


#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/effect/effect.h"
#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/rasterizer_state.h"
#include "azer/render/blending.h"
#include "ui/gfx/geometry/rect_f.h"
#include "ui/gfx/geometry/rect.h"

namespace azer {
class Renderer;
class OverlayEffect;
typedef scoped_refptr<OverlayEffect> OverlayEffectPtr;

class Overlay : public ::base::RefCounted<Overlay> {
 public:
  Overlay();
  virtual ~Overlay();

  Effect* effect();
  void Draw(Renderer* rs);
  void Render(Renderer* rs);

  void SetBounds(const gfx::RectF& rect);
  void SetTexBounds(const gfx::RectF& rect);
  void SetTexture(TextureView* tex);
  void UseBlending(BlendingPtr ptr) {blending_ = ptr;}
  void ResetBlending() { blending_ = NULL;}
 protected:
  SamplerStatePtr sampler_state_;
  BlendingPtr blending_;
  VertexBufferPtr vb_;
  OverlayEffectPtr effect_;
  TextureViewPtr texture_;
  Vector4 bounds_;
  Vector4 texbounds_;
  RasterizerStatePtr state_;
  DISALLOW_COPY_AND_ASSIGN(Overlay);
};

typedef scoped_refptr<Overlay> OverlayPtr;
}  // namespace azer
