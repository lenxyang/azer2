#pragma once

#include "base/basictypes.h"
#include "azer/render/effect.h"
#include "azer/render/overlay.h"
#include "azer/render/rasterizer_state.h"

namespace azer {
namespace d3d11 {

class D3DRenderSystem;

class D3DOverlayEffect : public Effect {
 public:
  static const char kEffectName[];
  D3DOverlayEffect();

  const char* GetEffectName() const override;
  bool Init(VertexDesc* desc, const TechSource&) override { return true;}
  void SetTexture(TexturePtr texture) { texture_ = texture;}
  void SetTransform(const Matrix4& matrx);
  void SetTexcoord(const Vector4 texcoord[4]);
  void SetVertex(const Vector4 vertex[4]);  
  
 protected:
  bool Init(Overlay* surface, D3DRenderSystem* rs);
  void UseTexture(Renderer* renderer) override;
  void ApplyGpuConstantTable(Renderer* renderer) override;
  static const char* kVertexShaderProg;
  static const char* kPixelShaderProg;
  TexturePtr texture_;
  friend class D3DOverlay;
  DISALLOW_COPY_AND_ASSIGN(D3DOverlayEffect);
};

class D3DOverlay : public Overlay {
 public:
  D3DOverlay(D3DRenderSystem* rs);
  virtual ~D3DOverlay();

  void Render(Renderer* renderer) override;

  static const int kVertexDescNum;
  static const azer::VertexDesc::Desc kVertexDesc[];
 protected:
  bool Init(azer::RenderSystem* rs);
  bool InitEffect();
  bool InitVertex(RenderSystem* rs);
  D3DRenderSystem* render_system_;
  RasterizerStatePtr rasterizer_state_;
  scoped_refptr<D3DOverlayEffect> effect_;
  friend class D3DRenderSystem;
  DISALLOW_COPY_AND_ASSIGN(D3DOverlay);
};
}  // namespace d3d11
}  // namespace azer
