#pragma once

#include <vector>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

#include "azer/base/export.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/blending.h"
#include "azer/render/depth_buffer.h"
#include "azer/render/gpu_program.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/render_target.h"
#include "azer/render/render_system_enum.h"
#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/viewport.h"

namespace azer {

class RenderSystem;

class AZER_EXPORT Renderer : public ::base::RefCounted<Renderer> {
 public:
  Renderer(RenderSystem* rs) : render_system_(rs) { }
  virtual ~Renderer() {}

  virtual const std::string& name() const = 0;

  virtual void Use() = 0;

  virtual bool IsDepthTestEnable() = 0;
  virtual void EnableDepthTest(bool enable) = 0;

  virtual FillMode GetFillMode(void) = 0;
  virtual void SetFillMode(FillMode mode) = 0;
  virtual CullingMode GetCullingMode(void) = 0;
  virtual void SetCullingMode(CullingMode mode) = 0;
  virtual FrontFace GetFrontFace() = 0;
  virtual void SetFrontFace(FrontFace mode) = 0;
  virtual void EnableMultisampleAntiAliasing(bool enable) = 0;
  virtual bool IsMultisampleAntiAliasingEnabled() = 0;
  virtual void EnableLineAntialiasing(bool enable) = 0;
  virtual bool IsLineAntialiasingEnabled() = 0;

  // save and restore the current state of renderer
  virtual void SaveState() = 0;
  virtual void RestoreState() = 0;

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
  DISALLOW_COPY_AND_ASSIGN(Renderer);
};

typedef scoped_refptr<Renderer> RendererPtr;
typedef std::vector<RendererPtr> RendererPtrVec;
}  // namespace azer
