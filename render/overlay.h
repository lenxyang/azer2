#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/effect.h"
#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/blending.h"
#include "ui/gfx/rect_f.h"
#include "ui/gfx/rect.h"

namespace azer {
class Renderer;

class AZER_EXPORT Overlay {
 public:
  /**
   * overlay �������� 3D ����ϵһ��
   * ���Խ�����Ϊ�� view volumn ������
   */
  virtual ~Overlay();
  virtual void Render(Renderer* rs) = 0;

  void SetTexture(TexturePtr tex) { tex_ = tex;}
  void SetBounds(const gfx::RectF& rect);
  void SetTexCoord(const gfx::PointF& lt, const gfx::PointF& br);
  void SetTransform(const Matrix4& matrix) { transform_ = matrix;}
  /**
   * Overlay �Ķ����ʽ
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

  Vector2 texcoord_[4];
  Vector4 vertex_[4];
  Matrix4 transform_;
  TexturePtr tex_;
  VertexBufferPtr vb_ptr_;
  VertexDescPtr vertex_desc_ptr_;
  static const int kVertexNum = 6;
  DISALLOW_COPY_AND_ASSIGN(Overlay);
};

typedef std::shared_ptr<Overlay> OverlayPtr;
}  // namespace azer
