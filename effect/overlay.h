#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/effect/effect.h"
#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/blending.h"
#include "ui/gfx/rect_f.h"
#include "ui/gfx/rect.h"

namespace azer {
class Renderer;
class OverlayEffect;

class Overlay : public ::base::RefCounted<Overlay> {
 public:
  Overlay();
  virtual ~Overlay();

  void Render(Renderer* rs);

  void SetBounds(const gfx::RectF& rect);
  void SetTexture(Texture* tex) { tex_ = tex;}
  void UseBlending(BlendingPtr ptr) {blending_ = ptr;}
  void ResetBlending() { blending_ = NULL;}
 protected:
  BlendingPtr blending_;
  VertexBufferPtr vb_;
  OverlayEffectPtr effect_;
  DISALLOW_COPY_AND_ASSIGN(Overlay);
};

typedef scoped_refptr<Overlay> OverlayPtr;
}  // namespace azer
