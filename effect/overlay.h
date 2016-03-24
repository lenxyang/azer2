#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/effect/effect.h"
#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/rasterizer_state.h"
#include "azer/render/blending.h"
#include "ui/gfx/rect_f.h"
#include "ui/gfx/rect.h"

namespace azer {
class Renderer;
class OverlayEffect;
typedef scoped_refptr<OverlayEffect> OverlayEffectPtr;

class Overlay : public ::base::RefCounted<Overlay> {
 public:
  Overlay();
  virtual ~Overlay();

  Effect* effect() { return effect_;} 
  void Render(Renderer* rs);

  void SetBounds(const gfx::RectF& rect);
  void SetTexBounds(const gfx::RectF& rect);
  void SetTexture(Texture* tex);
  void UseBlending(BlendingPtr ptr) {blending_ = ptr;}
  void ResetBlending() { blending_ = NULL;}
 protected:
  BlendingPtr blending_;
  VertexBufferPtr vb_;
  OverlayEffectPtr effect_;
  TexturePtr texture_;
  Vector4 bounds_;
  Vector4 texbounds_;
  RasterizerStatePtr state_;
  DISALLOW_COPY_AND_ASSIGN(Overlay);
};

typedef scoped_refptr<Overlay> OverlayPtr;
}  // namespace azer
