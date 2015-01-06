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
   * overlay 的坐标与 3D 坐标系一致
   * 可以将它认为是 view volumn 的切面
   */
  virtual ~Overlay();
  void Render(Renderer* rs);

  void SetTexture(TexturePtr tex);
  void SetTexCoord(const gfx::PointF& lt, const gfx::PointF& br) {
    texcoord1_ = lt;
    texcoord2_ = br;
  }
  void SetTransform(const Matrix4& matrix) { transform_ = matrix;}
  /**
   * Overlay 的顶点格式
   * struct Vertex {
   *   vec4 position;
   *   vec3 texcoord;
   * };
   */
  VertexDescPtr GetVertexDesc() { return vertex_desc_ptr_;}

  void EnableBlending(bool enabled) { blending_enabled_ = enabled;}
  bool IsBlendingEnable() const { return blending_enabled_;}
 protected:
  explicit Overlay(const gfx::RectF& rect);

  struct Vertex {
    azer::Vector4 position;
    azer::Vector2 texcoord;
    Vertex(const azer::Vector4& in_position, const azer::Vector2& in_texcoord)
        : position(in_position)
        , texcoord(in_texcoord){}
    Vertex() {}
  };

  virtual bool Init(RenderSystem* rs);

  void SetBlending(Renderer* renderer);
  void ResetBlending(Renderer* renderer);
  bool blending_enabled_;
  BlendingPtr blending_;

  Matrix4 transform_;
  gfx::PointF texcoord1_, texcoord2_;
  gfx::RectF rect_;
  VertexBufferPtr vb_ptr_;
  VertexDescPtr vertex_desc_ptr_;
  EffectPtr effect_ptr_;
  static const int kVertexNum = 6;
  DISALLOW_COPY_AND_ASSIGN(Overlay);
};

typedef std::shared_ptr<Overlay> OverlayPtr;
}  // namespace azer
