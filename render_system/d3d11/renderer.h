#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

#include <string>

#include "base/logging.h"
#include "azer/render/render_system.h"
#include "azer/render/shader.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/shader.h"

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

  void SetBlending(Blending* blending, float* factor, uint32 mask) override;
  void ResetBlending() override;
  void BindVertexBuffer(VertexBuffer* vb) override;
  void BindVertexBufferGroup(VertexBufferGroup* vbg) override;
  void BindIndicesBuffer(IndicesBuffer* ib) override;
  void BindConstantsTable(RenderPipelineStage stage, int32 index,
                          GpuConstantsTable* table) override;
  void ResetStageTexture(RenderPipelineStage stage) override;
  void SetShaderResTexture(RenderPipelineStage stage, int index, 
                           int32 count, TexturePtr* texture) override;
  void SetStreamOutTargets(HardwareBuffer** buffer, int count, int offset) override;
  void ResetShader(RenderPipelineStage stage) override;

  void Clear(const azer::Vector4& color) override;
  void ClearDepthAndStencil(bool clear_depth, bool clear_stencil, float depth_val, 
                            int stencil_val) override;
  
  void SetPrimitiveTopology(PrimitiveTopology primitive) override;
  void SetVertexLayout(VertexLayout* layout) override;
  void DrawAuto() override;
  void Draw(int32 num_of_vertices, int32 first_vertex) override;
  void DrawIndex(int32 num_of_indices, int32 first_indices, 
                 int32 index_base) override;
  void DrawInstanced(int32 instance_num, int32 num_of_vertices, int32 first_vertex,
                     int32 instance_start_index) override;
  void DrawIndexInstanced(int32 instance_num, int32 num_of_indices, 
                          int32 first_indices, int32 index_base,
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
  bool Init(RenderTarget* rt, DepthBuffer* depth);
  bool Init(std::vector<RenderTargetPtr>* rt, DepthBuffer* depth);
 protected:
  void InitRasterizerState();
  void InitForRenderTarget();
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

  bool InitForSurface(RenderTarget* target, DepthBuffer* depth);
 private:
  Surface* surface_;
  DISALLOW_COPY_AND_ASSIGN(D3DSurfaceRenderer);
};
}  // namespace d3d11
}  // namespace azer
