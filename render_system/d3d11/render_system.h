#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

#include <string>

#include "base/logging.h"
#include "azer/render/render_system.h"
#include "azer/render/gpu_program.h"
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
  RendererPtr CreateRenderer(const Texture::Options& opt) override;
  RendererPtrVec CreateRendererVec(const Texture::Options& opt,
                                   bool shared_depth_buffer) override;
  RendererPtr CreateDeferredRenderer(const Texture::Options& opt) override;
  BlendingPtr CreateBlending(const Blending::Desc& desc) override;
  VertexBufferGroupPtr CreateVertexBufferGroup(VertexDesc* desc) override;
  VertexBufferPtr CreateVertexBuffer(const VertexBuffer::Options& opt,
                                     SlotVertexData* data) override;
  IndicesBufferPtr CreateIndicesBuffer(const IndicesBuffer::Options& opt,
                                       IndicesData*) override;
  GpuConstantsTablePtr CreateGpuConstantsTable(
      int32 num, const GpuConstantsTable::Desc* desc) override;

  // texutre functions
  TexturePtr CreateTexture(const Texture::Options& opt) override;
  TexturePtr CreateTexture(const Texture::Options& opt, const Image* image) override;
  // RenderTarget* CreateRenderTarget(const Texture::Options& opt);
  // DepthBuffer* CreateDepthBuffer(const Texture::Options& opt);

  VertexGpuProgramPtr CreateVertexGpuProgram(
      VertexDescPtr desc, const GpuProgram::ShaderInfo& info) override;
  GpuProgramPtr CreateGpuProgram(RenderPipelineStage stage,
                                 const GpuProgram::ShaderInfo& info) override;
  OverlayPtr CreateOverlay() override;
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
