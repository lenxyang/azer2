#pragma once

#include "base/basictypes.h"
#include "azer/base/render_export.h"
#include "azer/render/effect.h"
#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"
#include "ui/gfx/rect_f.h"
#include "ui/gfx/rect.h"

namespace azer {
class Renderer;

class AZER_EXPORT OverlayEffect : public Effect {
 public:
  OverlayEffect(RenderSystem* rs) : Effect(rs) {}

  TexturePtr GetTexture() { return texture_ptr_;}
  void SetTexture(TexturePtr texture_ptr) { texture_ptr_ = texture_ptr;}

  virtual void Use(Renderer* rs);
 protected:
  TexturePtr texture_ptr_;
  DISALLOW_COPY_AND_ASSIGN(OverlayEffect);
};

typedef std::shared_ptr<OverlayEffect> OverlayEffectPtr;

class AZER_EXPORT Overlay {
 public:
  /**
   * overlay �������� 3D ����ϵһ��
   * ���Խ�����Ϊ�� view volumn ������
   */
  explicit Overlay(const gfx::RectF& rect) : rect_(rect) {}
  virtual ~Overlay() {}
  virtual void Render(Renderer* rs);

  virtual OverlayEffect* CreateDefaultEffect() = 0;

  EffectPtr& GetEffect() { return effect_ptr_;}
  void SetEffect(EffectPtr& ptr) { effect_ptr_ = ptr;}

  /**
   * Overlay �Ķ����ʽ
   * struct Vertex {
   *   vec4 position;
   *   vec3 texcoord;
   * };
   */
  VertexDescPtr GetVertexDesc() { return vertex_desc_ptr_;}
 protected:
  struct Vertex {
    azer::Vector4 position;
    azer::Vector2 texcoord;
    Vertex(const azer::Vector4& in_position, const azer::Vector2& in_texcoord)
        : position(in_position)
        , texcoord(in_texcoord){}
    Vertex() {}
  };

  gfx::RectF rect_;
  VertexBufferPtr vb_ptr_;
  VertexDescPtr vertex_desc_ptr_;
  EffectPtr effect_ptr_;
  static const int kVertexNum = 6;
  DISALLOW_COPY_AND_ASSIGN(Overlay);
};

typedef std::shared_ptr<Overlay> OverlayPtr;
}  // namespace azer
