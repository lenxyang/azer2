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

class AZER_EXPORT Overlay : public ::base::RefCounted<Resource> {
 public:
  /**
   * overlay 的坐标与 3D 坐标系一致
   * 可以将它认为是 view volumn 的切面
   */
  virtual ~Overlay();
  virtual void Render(Renderer* rs) = 0;

  void SetTexture(TexturePtr tex) { tex_ = tex;}
  void SetBounds(const gfx::RectF& rect);
  void SetTexCoord(const gfx::PointF& lt, const gfx::PointF& br);
  void SetTexCoord(const gfx::RectF& rc);
  void SetTransform(const Matrix4& matrix) { transform_ = matrix;}
  /**
   * Overlay 的顶点格式
   * struct Vertex {
   *   vec4 position;
   *   vec3 texcoord;
   * };
   */
  VertexDescPtr& GetVertexDesc() { return vertex_desc_ptr_;}

  void UseBlending(BlendingPtr ptr) {blending_ = ptr;}
  void UseDefaultBlending();
 protected:
  explicit Overlay();
  virtual bool Init(RenderSystem* rs);

  void SetBlending(Renderer* renderer);
  void ResetBlending(Renderer* renderer);
  BlendingPtr blending_;

  Vector4 texcoord_[4];
  Vector4 vertex_[4];
  Matrix4 transform_;
  TexturePtr tex_;
  VertexBufferPtr vb_ptr_;
  VertexDescPtr vertex_desc_ptr_;
  static const int kVertexNum = 6;
  DISALLOW_COPY_AND_ASSIGN(Overlay);
};

typedef scoped_refptr<Overlay> OverlayPtr;
}  // namespace azer
