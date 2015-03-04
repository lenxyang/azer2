#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "azer/math/math.h"
#include "azer/base/export.h"
#include "azer/base/string.h"
#include "azer/render/render_capability.h"
#include "azer/render/renderer.h"
#include "azer/render/surface.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/swap_chain.h"
#include "azer/render/reusable_object.h"
#include "ui/gfx/geometry/rect.h"

namespace azer {

class Canvas2D;
class Context2D;
class DepthRenderTarget;
class GpuConstantsTpable;
class Image;
class Overlay;
class RenderTarget;
class Technique;
class VertexBuffer;

typedef scoped_refptr<IndicesData> IndicesDataPtr;

class AZER_EXPORT RenderSystem {
 public:
  RenderSystem();

  virtual ~RenderSystem();
  virtual const StringType& name() const = 0;
  virtual const StringType& short_name() const = 0;
  static void SetRenderSystem(RenderSystem* rs);
  static RenderSystem* Current();

  virtual SwapChain* CreateSwapChainForSurface(Surface* surface) = 0;

  virtual Renderer* CreateRenderer(const Texture::Options& opt) = 0;
  virtual Renderer* CreateDeferredRenderer(const Texture::Options& opt) = 0;
  virtual Blending* CreateBlending(const Blending::Desc& desc) = 0;
  virtual Technique* CreateTechnique() = 0;

  // create gpu buffers
  virtual VertexBuffer* CreateVertexBuffer(const VertexBuffer::Options& opt,
                                           VertexData*) = 0;
  virtual IndicesBuffer* CreateIndicesBuffer(const IndicesBuffer::Options& opt,
                                             IndicesData*) = 0;
  virtual GpuConstantsTable* CreateGpuConstantsTable(
      int32 num, const GpuConstantsTable::Desc* desc) = 0;

  // textures functions
  virtual Texture* CreateTexture(const Texture::Options& opt) = 0;
  virtual Texture* CreateTexture(const Texture::Options& opt, const Image* image) = 0;
  // create GpuProgram
  // Vertex Gpu Program need to help check "Vertex Layout"
  //
  virtual VertexGpuProgram* CreateVertexGpuProgram(VertexDescPtr desc,
                                                   const std::string& program) = 0;
  virtual GpuProgram* CreateGpuProgram(RenderPipelineStage stage,
                                       const std::string& program) = 0;
  virtual Overlay* CreateOverlay() = 0;

  virtual bool reset() = 0;

  ReusableObject* GetReusableObject() { return reusable_object_.get();}

  Context2D* GetContext2D() { return context2d_.get();}
  const RenderSystemCapability& capability() const {
    return capability_;
  }
  static const int32 kMaxRenderTarget = 256;
 protected:
  // context2d, init by sub render-system
  RenderSystemCapability capability_;
  std::unique_ptr<Context2D> context2d_;

  ReusableObjectPtr reusable_object_;
  friend class AutoRenderSystemInit;
 private:
  static RenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(RenderSystem);
};
}  // namespace azer

