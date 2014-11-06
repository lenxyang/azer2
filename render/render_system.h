#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "azer/math/math.h"
#include "azer/base/render_export.h"
#include "azer/base/string.h"
#include "azer/render/render_capability.h"
#include "azer/render/renderer.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/swap_chain.h"
#include "ui/gfx/geometry/rect.h"

namespace azer {
class Overlay;
class WindowHost;
class Technique;
class VertexBuffer;
class RenderTarget;
class DepthRenderTarget;
class Image;
class AzerEGLInterface;

typedef std::shared_ptr<IndicesData> IndicesDataPtr;

class AZER_EXPORT RenderSystem {
 public:
  RenderSystem(WindowHost* host) : win_host_(host) { }

  virtual ~RenderSystem() {}
  virtual const StringType& name() const = 0;
  virtual const StringType& short_name() const = 0;

  static RenderSystem* InitRenderSystem(const ::base::FilePath& path,
                                        WindowHost* win);
  static void Release();
  static RenderSystem* Current();

  virtual Renderer* CreateRenderer(const Texture::Options& opt) = 0;
  virtual Renderer* CreateDeferredRenderer(const Texture::Options& opt) = 0;
  virtual Blending* CreateBlending(const Blending::Desc& desc) = 0;
  virtual Technique* CreateTechnique() = 0;

  /**
   * Create Vertex Buffer
   */
  virtual VertexBuffer* CreateVertexBuffer(const VertexBuffer::Options& opt,
                                           VertexData*) = 0;
  virtual IndicesBuffer* CreateIndicesBuffer(const IndicesBuffer::Options& opt,
                                             IndicesData*) = 0;
  virtual GpuConstantsTable* CreateGpuConstantsTable(
      int32 num, const GpuConstantsTable::Desc* desc) = 0;

  // textures functions
  virtual Texture* CreateTexture(const Texture::Options& opt) = 0;
  virtual Texture* CreateTexture(const Texture::Options& opt, const Image* image) = 0;
  // virtual RenderTarget* CreateRenderTarget(const Texture::Options& opt) = 0;
  // virtual DepthBuffer* CreateDepthBuffer(const Texture::Options& opt) = 0;  

  // create GpuProgram
  // Vertex Gpu Program need to help check "Vertex Layout"
  //
  virtual VertexGpuProgram* CreateVertexGpuProgram(VertexDescPtr desc,
                                                   const std::string& program) = 0;
  virtual GpuProgram* CreateGpuProgram(RenderPipelineStage stage,
                                       const std::string& program) = 0;
  virtual Overlay* CreateOverlay(const gfx::RectF& rect) = 0;

  virtual bool Present() = 0;
  virtual bool reset() = 0;

  // 
  virtual AzerEGLInterface* GetEGLInterface() = 0;

  const RenderSystemCapability& capability() const {
    return capability_;
  }

  WindowHost* GetWindowHost() const { return win_host_;}
  static const int32 kMaxRenderTarget = 256;

  Renderer* GetDefaultRenderer() {
    DCHECK(swap_chain_.get() != NULL);
    return swap_chain_->GetRenderer().get();
  }

  SwapChainPtr& GetSwapChain() { return swap_chain_;}
 protected:
  RenderSystemCapability capability_;
  SwapChainPtr swap_chain_;
  WindowHost* win_host_;
  friend class AutoRenderSystemInit;
  DISALLOW_COPY_AND_ASSIGN(RenderSystem);
};

}  // namespace azer

