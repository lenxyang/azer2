#pragma once

#include <d3d11.h>

#include "base/logging.h"
#include "azer/render/depth_buffer.h"

namespace azer {

class Surface;

namespace d3d11 {
class D3DRenderSystem;
class D3DRenderer;

class D3DDepthStencilState : public DepthStencilState {
 public:
  D3DDepthStencilState();
  ~D3DDepthStencilState() override;
  
  void EnableDepthTest(bool enable) override;
  bool IsDepthTestEnabled()  override;
  void SetDepthWriteMask(uint32 mask) override;
  void SetDepthCompareFunc(CompareFunc func) override;

  void EnableStencil(bool enable) override;
  bool IsStencilTestEnabled() override; 
  void SetStencilMask(uint8 read_mask, uint8 write_mask) override;
  void SetFrontFaceOper(const StencilOperStruct& oper) override;
  void SetBackFaceOper(const StencilOperStruct& oper) override;
  void Apply(Renderer* renderer, uint32 stencilref) override;
 private:
  void SetDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& desc);
  ID3D11DepthStencilState* depth_state_;
  DISALLOW_COPY_AND_ASSIGN(D3DDepthStencilState);
};

class D3DDepthBuffer : public DepthBuffer {
 public:
  static D3DDepthBuffer* Create(Surface* surface, D3DRenderSystem* rs);
  static D3DDepthBuffer* Create(const Texture::Options& opt, 
                                D3DRenderSystem* rs);
   // create by rendersystem
  D3DDepthBuffer(const Texture::Options& opt, D3DRenderSystem* rs);
  ~D3DDepthBuffer() override;

  bool Init(D3DRenderSystem* rs);
  void Clear(D3DRenderer*, bool clear_depth, bool clear_stencil, float depth_val,
             int stencil_val);

  ID3D11DepthStencilView* GetD3DDepthStencilView() { return target_;}
  ID3D11DepthStencilView* GetD3DDepthStencilViewRDOnly() { return rdonly_target_;}
 private:
  ID3D11DepthStencilView* target_;
  ID3D11DepthStencilView* rdonly_target_;
    D3DRenderSystem* render_system_;
  friend class D3DRenderSystem;
  DISALLOW_COPY_AND_ASSIGN(D3DDepthBuffer);
};
}  // namespace d3d11
}  // namespace azer
