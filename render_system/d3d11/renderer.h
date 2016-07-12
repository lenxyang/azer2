#pragma once

#include <string>

#include "base/logging.h"
#include "azer/render/render_system.h"
#include "azer/render/shader.h"
#include "azer/render_system/d3d11/d3d11.h"
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

  void SetBlending(Blending* blending, float* factor, uint32_t mask) override;
  void ResetBlending() override;
  void BindVertexBuffer(VertexBuffer* vb) override;
  void BindVertexBufferGroup(VertexBufferGroup* vbg) override;
  void BindIndicesBuffer(IndicesBuffer* ib) override;
  void BindConstantsTable(RenderPipelineStage stage, int index,
                          GpuConstantsTable* table) override;
  void ResetStageResource(RenderPipelineStage stage) override;
  void SetShaderResource(RenderPipelineStage stage, int index, 
                         int count, ResourceViewPtr* texture) override;
  void SetShaderUAResource(RenderPipelineStage stage, int index, 
                           int count, ResourceViewPtr* texture) override;
  void SetShaderSamplerState(RenderPipelineStage stage, int index, 
                             int count, SamplerStatePtr* sampler) override;
  void SetStreamOutTargets(GpuBuffer** buffer, int count, int offset) override;
  void ResetShader(RenderPipelineStage stage) override;
  void SetShader(int stage, Shader* shader) override;

  void Clear(const azer::Vector4& color) override;
  void ClearDepthAndStencil(bool clear_depth, bool clear_stencil, float depth_val, 
                            int stencil_val) override;
  
  void SetPrimitiveTopology(PrimitiveTopology primitive) override;
  void SetVertexLayout(VertexLayout* layout) override;
  void DrawAuto() override;
  void Draw(int num_of_vertices, int first_vertex) override;
  void DrawIndex(int num_of_indices, int first_indices, 
                 int index_base) override;
  void DrawInstanced(int instance_num, int num_of_vertices, 
                     int first_vertex, int instance_start_index) override;
  void DrawIndexInstanced(int instance_num, int num_of_indices, 
                          int first_indices, int index_base,
                          int instance_start_index) override;

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
  bool Init(std::vector<RenderTargetPtr>* rt, DepthBuffer* depth);
 protected:
  void InitRasterizerState();
  void InitForRenderTarget();

  D3DRenderSystem* d3d11_render_system_;
  Viewport viewport_;
  ID3D11DeviceContext* d3d_context_;
  static const std::string& name_;
  friend class RenderSystem;
  DISALLOW_COPY_AND_ASSIGN(D3DRenderer);
};

}  // namespace d3d11
}  // namespace azer
