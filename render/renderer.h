#pragma once

#include <vector>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

#include "azer/base/export.h"
#include "azer/render/depth_buffer.h"
#include "azer/render/render_state.h"
#include "azer/render/render_target.h"
#include "azer/render/render_system_enum.h"
#include "azer/render/viewport.h"

namespace azer {

class Blending;
class Effect;
class IndicesBuffer;
class GpuConstantsTable;
class RenderSystem;
class Texture;
class VertexBuffer;
class VertexBufferGroup;
class VertexLayout;


class AZER_EXPORT Renderer : public ::base::RefCounted<Renderer> {
 public:
  Renderer(RenderSystem* rs);
  virtual ~Renderer();

  virtual const std::string& name() const = 0;
  void ResetRenderState();
  void SetRenderState(RenderState* render_state);
  RenderState* GetRenderState();

  virtual bool IsDepthTestEnable() = 0;
  virtual void EnableDepthTest(bool enable) = 0;
  virtual void Use() = 0;

  void UseEffect(Effect* effect); 
  virtual void UseVertexBuffer(VertexBuffer* vb) = 0;
  virtual void UseVertexBufferGroup(VertexBufferGroup* vbg) = 0;
  virtual void UseIndicesBuffer(IndicesBuffer* ib) = 0;
  virtual void UseBlending(Blending* blending, float* factor,
                           uint32 mask = 0xffffffff) = 0;
  virtual void ResetBlending() = 0;

  virtual void UseTexture(RenderPipelineStage stage, int index, Texture* texture) = 0;

  virtual void UseConstantsTable(RenderPipelineStage stage,
                                 GpuConstantsTable* table) = 0;
  virtual void Reset() = 0;

  virtual void Clear(const azer::Vector4& color) = 0;

  virtual void ClearDepthAndStencil(
      DepthBuffer::ClearFlag flag = DepthBuffer::kClearAll,
      float depth_val = 1.0,int stencil_val = 0) = 0;

  virtual void ResetShader(RenderPipelineStage stage) = 0;
  virtual void SetVertexLayout(VertexLayout* layout) = 0;
  virtual void SetPrimitiveTopology(PrimitiveTopology primitive) = 0;
  virtual void Draw(int32 num_of_vertices = -1, int32 first_vertex = 0) = 0;
  virtual void DrawIndex(int32 num_of_indices, int32 first_indices = 0, 
                         int32 index_base = 0) = 0;
  virtual void DrawInstanced(int32 instance_num, int32 num_of_vertices = -1,
                             int32 first_vertex = 0, 
                             int32 instance_start_index = 0) = 0;
  virtual void DrawIndexInstanced(int32 instance_num, int32 num_of_indices = -1,
                                  int32 first_indices = 0, int32 index_base = 0,
                                  int32 instance_start_index = 0) = 0;
  
  virtual void SetViewport(const Viewport& viewport) = 0;
  virtual const Viewport& GetViewport() const  = 0;

  DepthBuffer* GetDepthBuffer() { return depth_.get();}
  RenderTarget* GetRenderTarget(int32 index) { return targets_[index].get();}
  typedef std::vector<RenderTargetPtr> RenderTargetVec;
  RenderTargetVec* GetAllRenderTargets() { return &targets_;}
  RenderSystem* GetRenderSystem() { return render_system_;}
 protected:
  RenderSystem* render_system_;
  DepthBufferPtr depth_;
  RenderTargetVec targets_;
  RenderStatePtr current_state_;
  RenderStatePtr default_state_;
  DISALLOW_COPY_AND_ASSIGN(Renderer);
};

typedef scoped_refptr<Renderer> RendererPtr;
typedef std::vector<RendererPtr> RendererPtrVec;

class ScopedRenderState {
 public:
  ScopedRenderState(Renderer* renderer, RenderState* new_state)
      : renderer_(renderer) {
    prev_state_ = renderer->GetRenderState();;
    renderer->SetRenderState(new_state);
  }
  ~ScopedRenderState() {
    renderer_->SetRenderState(prev_state_);
  }
 private:
  RenderStatePtr prev_state_;
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(ScopedRenderState);
};
}  // namespace azer
