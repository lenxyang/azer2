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

class VertexBuffer;

namespace d3d11 {
class D3DRenderTarget;
class D3DDepthBuffer;

class D3DRenderer : public Renderer {
 public:
  D3DRenderer(ID3D11DeviceContext* context, D3DRenderSystem* rs);
  ~D3DRenderer();

  void Reset() override;
  void Use() override;
  const std::string& name() const override;

  bool IsDepthTestEnable() override;
  void EnableDepthTest(bool enable) override;

  FillMode GetFillMode(void) override;
  void SetFillMode(FillMode mode) override;
  CullingMode GetCullingMode(void) override;
  void SetCullingMode(CullingMode mode) override;
  FrontFace GetFrontFace() override;
  void SetFrontFace(FrontFace mode) override;
  void EnableMultisampleAntiAliasing(bool enable) override;
  bool IsMultisampleAntiAliasingEnabled() override;
  void EnableLineAntialiasing(bool enable) override;
  bool IsLineAntialiasingEnabled() override;

  void SaveState() override;
  void RestoreState() override;

  void UseBlending(Blending* blending, float* factor, uint32 mask) override;
  void ResetBlending() override;

  void UseConstantsTable(RenderPipelineStage stage, 
                         GpuConstantsTable* table) override;

  // texutre functions
  void UseTexture(RenderPipelineStage stage, int index, Texture* texture) override;
  void ResetShader(RenderPipelineStage stage) override;

  void Clear(const azer::Vector4& color) override;
  void ClearDepthAndStencil(DepthBuffer::ClearFlag, float depth_val, 
                            int stencil_val) override;
  
  void Draw(VertexBuffer* vb, PrimitiveTopology primitive,
            int32 num_of_vertices, int32 first_vertex) override;
  void DrawIndex(VertexBuffer* vb, IndicesBuffer* ib,
                 PrimitiveTopology primitive, int32 num_of_indices,
                 int32 first_indices, int32 index_base) override;
  void DrawInstanced(int32 instance_num, VertexBuffer* vb,
                     PrimitiveTopology primitive,
                     int32 num_of_vertices, int32 first_vertex,
                     int32 instance_start_index) override;
  void DrawIndexInstanced(int32 instance_num, VertexBuffer* vb,
                          IndicesBuffer* ib, PrimitiveTopology primitive,
                          int32 num_of_indices, int32 first_indices,
                          int32 index_base,
                          int32 instance_start_index) override;

  void SetViewport(const Viewport& viewport) override;
  const Viewport& GetViewport() const override;

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
  bool Init(RenderTargetPtr rt, DepthBufferPtr depth);
 protected:
  void InitRenderState();
  void InitForRenderTarget();
  void ResetTexture(RenderPipelineStage stage, int index);
  void SetShaderResource(RenderPipelineStage stage, uint32 first, uint32 num,
                         ID3D11ShaderResourceView** view);

  D3DRenderSystem* d3d11_render_system_;
  Viewport viewport_;
  ID3D11DeviceContext* d3d_context_;
  static const std::string& name_;
  friend class RenderSystem;
  DISALLOW_COPY_AND_ASSIGN(D3DRenderer);
};

class D3DSurfaceRenderer : public D3DRenderer {
 public:
  D3DSurfaceRenderer(Surface* surface, ID3D11DeviceContext* context,
                     D3DRenderSystem* rs)
      : D3DRenderer(context, rs)
      , surface_(surface) {
    DCHECK(NULL != surface);
  }

  bool InitForSurface(RenderTargetPtr target, DepthBufferPtr depth);
 private:
  Surface* surface_;
  DISALLOW_COPY_AND_ASSIGN(D3DSurfaceRenderer);
};
}  // namespace d3d11
}  // namespace azer
