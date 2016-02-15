#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "azer/math/math.h"
#include "azer/base/export.h"
#include "azer/base/string.h"
#include "azer/render/blending.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/render_capability.h"
#include "azer/render/renderer.h"
#include "azer/render/shader.h"
#include "azer/render/swap_chain.h"
#include "azer/render/surface.h"
#include "azer/render/vertex_buffer.h"
#include "ui/gfx/geometry/rect.h"

namespace azer {

class Canvas2D;
class Context2D;
class GpuComputeTaskDispatcher;
class GpuConstantsTable;
class Image;
class Overlay;
class RenderTarget;
class Technique;
class RasterizerState;

typedef scoped_refptr<Blending> BlendingPtr;
typedef scoped_refptr<GpuComputeTaskDispatcher> GpuComputeTaskDispatcherPtr;
typedef scoped_refptr<GpuConstantsTable> GpuConstantsTpablePtr;;
typedef scoped_refptr<Overlay> OverlayPtr;
typedef scoped_refptr<Renderer> RendererPtr;
typedef scoped_refptr<RenderTarget> RenderTargetPtr;
typedef scoped_refptr<RasterizerState> RasterizerStatePtr;
typedef scoped_refptr<SwapChain> SwapChainPtr;
typedef scoped_refptr<Technique> TechniquePtr;

class AZER_EXPORT RenderSystem {
 public:
  RenderSystem();

  virtual ~RenderSystem();
  virtual const StringType& name() const = 0;
  virtual const StringType& short_name() const = 0;
  static void SetRenderSystem(RenderSystem* rs);
  static RenderSystem* Current();

  virtual SwapChainPtr CreateSwapChainForSurface(Surface* surface) = 0;

  virtual RasterizerStatePtr CreateRasterizerState() = 0;
  virtual DepthStencilStatePtr CreateDepthStencilState() = 0;
  virtual RendererPtr CreateRenderer(const Texture::Options& opt) = 0;
  virtual RendererPtrVec CreateRendererVec(const Texture::Options& opt,
                                           bool shared_depth_buffer) = 0;
  virtual RendererPtr CreateDeferredRenderer(const Texture::Options& opt) = 0;
  virtual BlendingPtr CreateBlending(const Blending::Desc& desc) = 0;
  virtual TechniquePtr CreateTechnique() = 0;

  // create gpu buffers
  virtual VertexBufferGroupPtr CreateVertexBufferGroup(VertexDesc* desc) = 0;
  virtual VertexBufferPtr CreateVertexBuffer(const HBufferOptions& opt,
                                             SlotVertexData* vdata) = 0;
  virtual IndicesBufferPtr CreateIndicesBuffer(const HBufferOptions& opt,
                                               IndicesData* idata) = 0;
  virtual GpuConstantsTablePtr CreateGpuConstantsTable(
      int32 num, const GpuConstantsTable::Desc* desc) = 0;

  // textures functions
  virtual TexturePtr CreateTexture(const Texture::Options& opt) = 0;
  virtual TexturePtr CreateTexture(const Texture::Options& opt, const Image* image) = 0;

  // create Shader
  // Vertex Gpu Program need to help check "Vertex Layout"
  //
  virtual ShaderPtr CreateVertexShader(VertexDesc* desc, const ShaderInfo& info) = 0;
  virtual ShaderPtr CreateShader(const ShaderInfo& info) = 0;
  virtual GpuComputeTaskDispatcherPtr CreateDispatcher() = 0;
  virtual OverlayPtr CreateOverlay() = 0;

  virtual bool reset() = 0;

  Context2D* GetContext2D() { return context2d_.get();}
  const RenderSystemCapability& capability() const {
    return capability_;
  }
  static const int32 kMaxRenderTarget = 256;
 protected:
  // context2d, init by sub render-system
  RenderSystemCapability capability_;
  std::unique_ptr<Context2D> context2d_;

  friend class AutoRenderSystemInit;
 private:
  static RenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(RenderSystem);
};
}  // namespace azer

