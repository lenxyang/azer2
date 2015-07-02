#pragma once

#include <vector>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "ui/gfx/geometry/rect.h"

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

namespace azer {

class RenderSystem;

class AZER_EXPORT Renderer : public ::base::RefCounted<Renderer> {
 public:
  Renderer(RenderSystem* rs) : render_system_(rs) { }
  virtual ~Renderer() {}

  virtual const std::string& name() const = 0;

  virtual void Use() = 0;
  virtual void Use(int index) = 0;

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

  virtual void Draw(VertexBuffer* vb, PrimitiveTopology primitive,
                    int32 num_of_vertices = -1, int32 first_vertex = 0) = 0;
  virtual void DrawIndex(VertexBuffer* vb, IndicesBuffer* ib,
                         PrimitiveTopology primitive, int32 num_of_indices = -1,
                         int32 first_indices = 0, int32 index_base = 0) = 0;
  virtual void DrawInstanced(int32 instance_num, VertexBuffer* vb,
                             PrimitiveTopology primitive, int32 num_of_vertices = -1,
                             int32 first_vertex = 0,
                             int32 instance_start_index = 0) = 0;
  virtual void DrawIndexInstanced(int32 instance_num, VertexBuffer* vb,
                                  IndicesBuffer* ib, PrimitiveTopology primitive,
                                  int32 num_of_indices = -1,
                                  int32 first_indices = 0, int32 index_base = 0,
                                  int32 instance_start_index = 0) = 0;
  
  struct Viewport {
    gfx::Rect bounds;
    int32 min_depth;
    int32 max_depth;

    Viewport(int32 l, int32 t, int32 w, int32 h, float mindepth = 0.0f,
             float maxdepth = 1.0f)
        : bounds(l, t, w, h)
        , min_depth(mindepth), max_depth(maxdepth) {
    }

    Viewport(const gfx::Rect& rect, float mindepth = 0.0f, float maxdepth = 1.0f)
        : bounds(rect)
        , min_depth(mindepth), max_depth(maxdepth) {
    }

    Viewport()
        : bounds(0, 0, 800, 600)
        , min_depth(0.0f), max_depth(1.0f) {
    }
  };
  virtual void SetViewport(const Viewport& viewport) = 0;
  virtual const Viewport& GetViewport() const  = 0;

  DepthBufferPtr& GetDepthBuffer() { return depth_;}
  RenderTargetPtr& GetRenderTarget() { return targets_[0];}
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
}  // namespace azer
