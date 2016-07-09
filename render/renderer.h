#pragma once

#include <vector>
#include <string>


#include "base/memory/ref_counted.h"

#include "azer/base/export.h"
#include "azer/render/render_target.h"
#include "azer/render/common.h"
#include "azer/render/viewport.h"

namespace azer {

class Blending;
class Effect;
class DepthBuffer;
class DepthStencilState;
class GpuBuffer;
class GpuConstantsTable;
class IndicesBuffer;
class RenderSystem;
class RasterizerState;
class Texture;
class TextureView;
class VertexBuffer;
class VertexBufferGroup;
class VertexLayout;

typedef scoped_refptr<DepthBuffer> DepthBufferPtr;
typedef scoped_refptr<DepthStencilState> DepthStencilStatePtr;
typedef scoped_refptr<RasterizerState> RasterizerStatePtr;
typedef scoped_refptr<TextureView> TextureViewPtr;


class AZER_EXPORT Renderer : public ::base::RefCounted<Renderer> {
 public:
  Renderer(RenderSystem* rs);
  virtual ~Renderer();

  virtual const std::string& name() const = 0;
  void ResetRasterizerState();
  void SetRasterizerState(RasterizerState* render_state);
  RasterizerState* GetRasterizerState();

  uint32_t stencilref() { return stencilref_;}
  void ResetDepthStencilState();
  void SetDepthStencilState(DepthStencilState* render_state, uint32_t stencilref);
  DepthStencilState* GetDepthStencilState();

  virtual void Use() = 0;

  void BindEffect(Effect* effect); 
  virtual void BindVertexBuffer(VertexBuffer* vb) = 0;
  virtual void BindVertexBufferGroup(VertexBufferGroup* vbg) = 0;
  virtual void BindIndicesBuffer(IndicesBuffer* ib) = 0;
  virtual void ResetStageTexture(RenderPipelineStage stage) = 0;
  virtual void SetShaderResTexture(RenderPipelineStage stage, int index, 
                                   int32_t count, TextureViewPtr* texture) = 0;
  virtual void SetShaderSamplerState(RenderPipelineStage stage, int index, 
                                     int32_t count, SamplerStatePtr* sampler) = 0;
  virtual void BindConstantsTable(RenderPipelineStage stage, int32_t index,
                                  GpuConstantsTable* table) = 0;
  virtual void SetStreamOutTargets(GpuBuffer** buffer, int count, int off) = 0;
  virtual void SetBlending(Blending* blending, float* factor, uint32_t mask) = 0;
  virtual void ResetBlending() = 0;
  virtual void Reset() = 0;

  virtual void Clear(const azer::Vector4& color) = 0;

  virtual void ClearDepthAndStencil(bool clear_depth = true,
                                    bool clear_stencil = false,
                                    float depth_val = 1.0,
                                    int stencil_val = 0) = 0;

  virtual void ResetShader(RenderPipelineStage stage) = 0;
  virtual void SetVertexLayout(VertexLayout* layout) = 0;
  virtual void SetPrimitiveTopology(PrimitiveTopology primitive) = 0;
  virtual void DrawAuto() = 0;
  virtual void Draw(int32_t num_of_vertices, int32_t first_vertex) = 0;
  virtual void DrawIndex(int32_t indices_count, int32_t vbase, int32_t ibase) = 0;
  virtual void DrawInstanced(int32_t instance_count, int32_t vertices_count,
                             int32_t vertex_base, int32_t instance_start_index) = 0;
  virtual void DrawIndexInstanced(int32_t instance_num, int32_t num_of_indices,
                                  int32_t first_indices, int32_t index_base,
                                  int32_t instance_start_index) = 0;
  
  const gfx::Size& size() const; 
  virtual void SetViewport(const Viewport& viewport) = 0;
  virtual const Viewport& GetViewport() const  = 0;

  DepthBuffer* GetDepthBuffer() { return depth_.get();}
  RenderTarget* GetRenderTarget(int32_t index) { return targets_[index].get();}
  typedef std::vector<RenderTargetPtr> RenderTargetVec;
  RenderTargetVec* GetAllRenderTargets() { return &targets_;}
  RenderSystem* GetRenderSystem() { return render_system_;}

  void SetDepthBuffer(DepthBuffer* depth);
  void SetRenderTargets(std::vector<RenderTargetPtr>* targets);
 protected:
  RenderSystem* render_system_;
  DepthBufferPtr depth_;
  RenderTargetVec targets_;
  RasterizerStatePtr current_rasterizer_state_;
  RasterizerStatePtr default_rasterizer_state_;
  DepthStencilStatePtr current_depth_state_;
  DepthStencilStatePtr default_depth_state_;
  uint32_t stencilref_;
  DISALLOW_COPY_AND_ASSIGN(Renderer);
};

typedef scoped_refptr<Renderer> RendererPtr;
typedef std::vector<RendererPtr> RendererPtrVec;

class ScopedRenderer {
 public:
  ScopedRenderer(Renderer* renderer) 
      : renderer_(renderer) {
    renderer_->Use();
  }
  ~ScopedRenderer() {
    renderer_->Reset();
  }
 private:
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(ScopedRenderer);
};

RendererPtr CreateCommonRenderer(const gfx::Size& size);
}  // namespace azer
