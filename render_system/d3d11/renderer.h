#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

#include <string>

#include "base/logging.h"
#include "azer/render/render_system.h"
#include "azer/render/gpu_program.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/gpu_program.h"

namespace azer {

class WindowHost;
class VertexBuffer;
class D3D11RenderTarget;
class D3D11DepthBuffer;

class D3D11Renderer : public Renderer {
 public:
  D3D11Renderer(ID3D11DeviceContext* context, D3D11RenderSystem* rs);
  ~D3D11Renderer();

  virtual void Reset() override;
  virtual void Use() override;
  virtual const std::string& name() const override;

  virtual bool IsDepthTestEnable() override;
  virtual void EnableDepthTest(bool enable) override;

  virtual FillMode GetFillMode(void) override;
  virtual void SetFillMode(FillMode mode) override;
  virtual CullingMode GetCullingMode(void) override;
  virtual void SetCullingMode(CullingMode mode) override;
  virtual FrontFace GetFrontFace() override;
  virtual void SetFrontFace(FrontFace mode) override;
  virtual void EnableMultisampleAntiAliasing(bool enable) override;
  virtual bool IsMultisampleAntiAliasingEnabled() override;
  virtual void EnableLineAntialiasing(bool enable) override;
  virtual bool IsLineAntialiasingEnabled() override;

  virtual void UseBlending(Blending* blending, float* factor, uint32 mask) override;
  virtual void ResetBlending() override;

  virtual void UseConstantsTable(RenderPipelineStage stage,
                                 GpuConstantsTable* table) override;

  // texutre functions
  virtual void UseTexture(RenderPipelineStage stage, int index, Texture* texture) override;
  virtual void ResetShader(RenderPipelineStage stage) override;

  virtual void Clear(const azer::Vector4& color) override;
  virtual void ClearDepthAndStencil(DepthBuffer::ClearFlag,
                                    float depth_val, int stencil_val) override;
  
  virtual void Draw(VertexBuffer* vb, PrimitiveTopology primitive,
                      int32 num_of_vertices, int32 first_vertex) override;
  virtual void DrawIndex(VertexBuffer* vb, IndicesBuffer* ib,
                         PrimitiveTopology primitive, int32 num_of_indices,
                         int32 first_indices, int32 index_base) override;
  virtual void DrawInstanced(int32 instance_num, VertexBuffer* vb,
                             PrimitiveTopology primitive,
                             int32 num_of_vertices, int32 first_vertex,
                             int32 instance_start_index) override;
  virtual void DrawIndexInstanced(int32 instance_num, VertexBuffer* vb,
                                  IndicesBuffer* ib, PrimitiveTopology primitive,
                                  int32 num_of_indices, int32 first_indices,
                                  int32 index_base,
                                  int32 instance_start_index) override;

  virtual void SetViewport(const Viewport& viewport) override;
  
  ID3D11DeviceContext* GetContext() {
    CHECK(NULL != d3d_context_);
    return d3d_context_;
  }

  ID3D11Device* GetDevice() {
    DCHECK(NULL != d3d11_render_system_);
    return d3d11_render_system_->GetDevice();
  }

  void ResetRenderTarget();

  bool Init(const Texture::Options& opt);
 protected:
  void InitRenderState();
  void InitForRenderTarget();
  void ResetTexture(RenderPipelineStage stage, int index);
  void SetShaderResource(RenderPipelineStage stage, uint32 first, uint32 num,
                         ID3D11ShaderResourceView** view);

  D3D11RenderSystem* d3d11_render_system_;
  Viewport viewport_;
  ID3D11DeviceContext* d3d_context_;
  static const std::string& name_;
  friend class RenderSystem;
  DISALLOW_COPY_AND_ASSIGN(D3D11Renderer);
};

class D3D11SurfaceRenderer : public D3D11Renderer {
 public:
  D3D11SurfaceRenderer(Surface* surface,
                       ID3D11DeviceContext* context,
                       D3D11RenderSystem* rs)
      : D3D11Renderer(context, rs)
      , surface_(surface) {
    DCHECK(NULL != surface);
  }

  bool InitForSurface(RenderTargetPtr target, DepthBufferPtr depth);
 private:
  Surface* surface_;
  DISALLOW_COPY_AND_ASSIGN(D3D11SurfaceRenderer);
};
}  // namespace azer
