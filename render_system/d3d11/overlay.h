#pragma once

#include "base/basictypes.h"
#include "azer/render/overlay.h"

namespace azer {
namespace d3d11 {

class D3DRenderSystem;

class D3DOverlayEffect : Effect {
 public:
  D3DOverlayEffect(D3DRenderSystem* rs)
      : Effect((RenderSystem*)rs) {
  }

#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 transform;
    Vector2 tex[4];
  };
#pragma pack(pop)
  
  void SetTexture(TexturePtr texture) { texture_ = texture;}
  void SetTransform(const Matrix4& matrx);
  void SetTexcoord(const Vector2 texcoord[4]);

  void Use(Renderer* renderer) override;

  void UseTexture(Renderer* renderer) override {}
 protected:
  bool Init(Overlay* surface, D3DRenderSystem* rs);
  static const char* kVertexShaderProg;
  static const char* kPixelShaderProg;

  Vector2 texcoord_[4];
  Matrix4 transform_;
  TexturePtr texture_;
  friend class D3DOverlay;
  DISALLOW_COPY_AND_ASSIGN(D3DOverlayEffect);
};

class D3DOverlay : public Overlay {
 public:
  D3DOverlay(const gfx::RectF& rect, D3DRenderSystem* rs)
      : Overlay(rect)
      , render_system_(rs) {}
  virtual ~D3DOverlay() {}

  void Render(Renderer* renderer) override;

  static const int kVertexDescNum;
  static const azer::VertexDesc::Desc kVertexDesc[];
 protected:
  struct Vertex {
    azer::Vector4 position;
    azer::Vector2 texcoord;
    int32 index;
    Vertex(const azer::Vector4& in_position, const azer::Vector2& in_texcoord)
        : position(in_position)
        , texcoord(in_texcoord) 
        , index(0) {
    }

    Vertex() {
    }
  };

  bool Init(azer::RenderSystem* rs);
  bool InitEffect();
  bool InitVertex(RenderSystem* rs);
  D3DRenderSystem* render_system_;
  std::unique_ptr<D3DOverlayEffect> effect_;
  friend class D3DRenderSystem;
  DISALLOW_COPY_AND_ASSIGN(D3DOverlay);
};
}  // namespace d3d11
}  // namespace azer
