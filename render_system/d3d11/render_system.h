#pragma once

#include <string>

#include "base/logging.h"
#include "azer/render/render_system.h"
#include "azer/render/shader.h"
#include "azer/render_system/d3d11/swap_chain.h"
#include "azer/render_system/d3d11/d3denv.h"

namespace azer {

class VertexBuffer;
class Canvas2D;

namespace d3d11 {
class D3DRenderTarget;
class D3DDepthBuffer;

class D3DRenderSystem : public RenderSystem {
 public:
  D3DRenderSystem(D3DEnvironmentPtr envptr);
  ~D3DRenderSystem();

  bool Init();
  const StringType& name() const override;
  const StringType& short_name() const override;

  SwapChainPtr CreateSwapChainForSurface(Surface* surface) override;

  RasterizerStatePtr CreateRasterizerState() override;
  DepthStencilStatePtr CreateDepthStencilState() override;
  DepthBufferPtr CreateDepthBuffer(const DepthBuffer::Options& opt, 
                                   Texture* texture) override;
  RenderTargetPtr CreateRenderTarget(const RenderTarget::Options& opt, 
                                     Texture* texture) override;

  RendererPtr CreateRenderer(RenderTargetPtrs* targets, DepthBuffer* depth) override;
  BlendingPtr CreateBlending(const Blending::BlendDesc& desc) override;
  VertexBufferGroupPtr CreateVertexBufferGroup(VertexDesc* desc) override;
  VertexBufferPtr CreateVertexBuffer(const GpuResOptions& opt,
                                     SlotVertexData* data) override;
  IndicesBufferPtr CreateIndicesBuffer(const GpuResOptions& opt,
                                       IndicesData*) override;
  ShaderParamTablePtr CreateShaderParamTable(
      int32_t num, const ShaderParamTable::Desc* desc) override;

  // texutre functions
  TexturePtr CreateTexture(const Texture::Options& opt) override;
  TexturePtr CreateTexture(const Texture::Options& opt,
                           const ImageData* image) override;
  SamplerStatePtr CreateSamplerState(const SamplerState::Options& opt) override;
  TextureViewPtr CreateTextureView(const TextureView::Options& opt, 
                                   Texture* tex) override;
  TextureUAViewPtr CreateTextureUAView(const TextureUAView::Options& opt, 
                                       Texture* tex) override;
  StructuredGpuBufferPtr CreateStructuredBuffer(
      const GpuResOptions& opt, int count, int strip) override;
  StructuredGpuBufferPtr CreateStructuredBufferWithData(
      const GpuResOptions& o, int count, int strip, const uint8_t* data) override;
  ShaderResViewPtr CreateStructBufferShaderResView(GpuResource* buffer) override;
  UnorderAccessResViewPtr CreateStructBufferUAResView(GpuResource* buffer) override;

  ShaderPtr CreateShader(const ShaderInfo& info, VertexDesc* desc) override;
  TechniquePtr CreateTechnique() override;

  bool reset() override;

  D3DEnvironmentPtr& GetD3DEnv() { return envptr_;}
  /**
   * direct3d relevent
   */
  ID3D11Device* GetDevice() { return envptr_->GetDevice();}
  ID3D11DeviceContext* GetContext() { return envptr_->GetContext();}
  const D3D_FEATURE_LEVEL& feature_level() const { return envptr_->feature_level();}
  void ResetRenderTarget();
 protected:
  void InitContext2D();
  void GetDriverCapability();
  bool InitSwapchainRenderer();

  D3DEnvironmentPtr envptr_;
  static const StringType& name_;
  static const StringType& short_name_;
  DISALLOW_COPY_AND_ASSIGN(D3DRenderSystem);
};
}  // namespace d3d11
}  // namespace azer
