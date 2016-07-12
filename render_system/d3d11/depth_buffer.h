#pragma once

#include "base/logging.h"
#include "azer/render/depth_buffer.h"
#include "azer/render_system/d3d11/d3d11.h"

namespace azer {

class Surface;

namespace d3d11 {
class D3DTexture;
class D3DRenderSystem;
class D3DRenderer;

class D3DDepthStencilState : public DepthStencilState {
 public:
  D3DDepthStencilState();
  ~D3DDepthStencilState() override;
  
  void EnableDepthTest(bool enable) override;
  bool IsDepthTestEnabled()  override;
  void SetDepthWriteMask(uint32_t mask) override;
  void SetDepthCompareFunc(CompareFunc func) override;

  void EnableStencil(bool enable) override;
  bool IsStencilTestEnabled() override; 
  void SetStencilMask(uint8_t read_mask, uint8_t write_mask) override;
  void SetFrontFaceOper(const StencilOperStruct& oper) override;
  void SetBackFaceOper(const StencilOperStruct& oper) override;
  void Apply(Renderer* renderer, uint32_t stencilref) override;
 private:
  void SetDepthStencilState(const D3D11_DEPTH_STENCIL_DESC& desc);
  ID3D11DepthStencilState* depth_state_;
  DISALLOW_COPY_AND_ASSIGN(D3DDepthStencilState);
};

class D3DDepthBuffer : public DepthBuffer {
 public:
   // create by rendersystem
  D3DDepthBuffer(const Options& opt, D3DRenderSystem* rs);
  ~D3DDepthBuffer() override;

  bool Init(D3DTexture* tex);
  void Clear(D3DRenderer*, bool clear_depth, bool clear_stencil, float depth_val,
             int stencil_val);

  ID3D11DepthStencilView* GetD3DDepthStencilView() { return target_;}
  NativeGpuResourceViewHandle native_handle() override;
 private:
  ID3D11DepthStencilView* target_;
  ID3D11Resource* texres_;
  D3DRenderSystem* render_system_;
  friend class D3DRenderSystem;
  DISALLOW_COPY_AND_ASSIGN(D3DDepthBuffer);
};
}  // namespace d3d11
}  // namespace azer
