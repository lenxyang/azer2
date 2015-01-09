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


class WindowHost;
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
  virtual const StringType& name() const override;
  virtual const StringType& short_name() const override;

  virtual Renderer* CreateRenderer(const Texture::Options& opt) override;
  virtual Renderer* CreateDeferredRenderer(const Texture::Options& opt) override;
  virtual Blending* CreateBlending(const Blending::Desc& desc) override;
  virtual VertexBuffer* CreateVertexBuffer(const VertexBuffer::Options& opt,
                                           VertexData*) override;
  virtual IndicesBuffer* CreateIndicesBuffer(const IndicesBuffer::Options& opt,
                                             IndicesData*) override;
  virtual GpuConstantsTable* CreateGpuConstantsTable(
      int32 num, const GpuConstantsTable::Desc* desc) override;

  // texutre functions
  virtual Texture* CreateTexture(const Texture::Options& opt) override;
  virtual Texture* CreateTexture(const Texture::Options& opt,
                                 const Image* image) override;
  // virtual RenderTarget* CreateRenderTarget(const Texture::Options& opt);
  // virtual DepthBuffer* CreateDepthBuffer(const Texture::Options& opt);

  virtual VertexGpuProgram* CreateVertexGpuProgram(VertexDescPtr desc,
                                                   const std::string& str) override;
  virtual GpuProgram* CreateGpuProgram(RenderPipelineStage stage,
                                       const std::string& program) override;
  virtual Overlay* CreateOverlay() override;
  virtual Technique* CreateTechnique() override;

  virtual bool Present() override;
  virtual bool reset() override;

  RenderSystemCapability ResetCapability() override;

  D3DEnvironmentPtr& GetD3DEnv() { return envptr_;}
  /**
   * direct3d relevent
   */
  ID3D11Device* GetDevice() { return envptr_->GetDevice();}
  ID3D11DeviceContext* GetContext() { return envptr_->GetContext();}
  const D3D_FEATURE_LEVEL& feature_level() const { return envptr_->feature_level();}
  void ResetRenderTarget();
 protected:
  Context2D* InitContext2D() override;
  void GetDriverCapability();
  bool InitD3DDevice();
  bool InitDefaultRenderer();

  D3DEnvironmentPtr envptr_;
  static const StringType& name_;
  static const StringType& short_name_;
  DISALLOW_COPY_AND_ASSIGN(D3DRenderSystem);
};
}  // namespace d3d11
}  // namespace azer
