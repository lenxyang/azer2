#include "azer/render_system/d3d11/render_system.h"

#include <string>


#include "base/strings/string16.h"
#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/base/image.h"
#include "azer/render/render_system_enum.h"
#include "azer/render_system/d3d11/util.h"
#include "azer/render_system/d3d11/blending.h"
#include "azer/render_system/d3d11/canvas2d.h"
#include "azer/render_system/d3d11/depth_buffer.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_constants_table.h"
#include "azer/render_system/d3d11/gpu_program.h"
#include "azer/render_system/d3d11/indices_buffer.h"
#include "azer/render_system/d3d11/render_target.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/overlay.h"
#include "azer/render_system/d3d11/technique.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/vertex_buffer.h"

namespace azer {
namespace d3d11 {

const StringType& D3DRenderSystem::name_ = AZER_LITERAL("Direct3D11RenderSystem");
const StringType& D3DRenderSystem::short_name_ = AZER_LITERAL("d3d11");

D3DRenderSystem::D3DRenderSystem(D3DEnvironmentPtr envptr)
    : envptr_(envptr)
    , RenderSystem(envptr->GetSurface()) {
}

D3DRenderSystem::~D3DRenderSystem() {
}

bool D3DRenderSystem::Init() {
  std::unique_ptr<D3DSwapChain> ptr(new D3DSwapChain(this));
  if (!ptr->Init(envptr_->GetSurface())) {
    return false;
  }

  swap_chain_.reset(ptr.release());
  GetDefaultRenderer()->Use();
  return true;
}

bool D3DRenderSystem::Present() {
  DCHECK(swap_chain_ != NULL) << "swap_chain cannto be NULL";
  if (!swap_chain_->Present()) {
    LOG(ERROR) << " failed to Present.";

    return swap_chain_->reset(envptr_->GetSurface());
  }

  return true;
}

bool D3DRenderSystem::reset() {
  DCHECK(swap_chain_ != NULL);
  return swap_chain_->reset(envptr_->GetSurface());
}

void D3DRenderSystem::GetDriverCapability() {
  capability_.hw_transform_light_ = true;
}

const StringType& D3DRenderSystem::name() const {
  return name_;
}

const StringType& D3DRenderSystem::short_name() const {
  return short_name_;
}

VertexBuffer* D3DRenderSystem::CreateVertexBuffer(
    const VertexBuffer::Options& opt, VertexData* dataptr) {
  std::unique_ptr<D3DVertexBuffer> vertex_buffer(
      new D3DVertexBuffer(opt, this));
  if (vertex_buffer->Init(dataptr)) {
    return vertex_buffer.release();
  } else {
    return NULL;
  }
}


IndicesBuffer* D3DRenderSystem::CreateIndicesBuffer(
    const IndicesBuffer::Options& opt, IndicesData* dataptr) {
  std::unique_ptr<D3DIndicesBuffer> indices_buffer(
      new D3DIndicesBuffer(opt, this));
  if (indices_buffer->Init(dataptr)) {
    return indices_buffer.release();
  } else {
    return NULL;
  }
}

GpuProgram* D3DRenderSystem::CreateGpuProgram(RenderPipelineStage stage,
                                              const std::string& program) {
  std::unique_ptr<GpuProgram> gpu_program;
  switch (stage) {
    case kPixelStage:
      gpu_program.reset(new D3DPixelGpuProgram(program));
      break;
    case kGeometryStage:
      gpu_program.reset(new D3DGeometryGpuProgram(program));
      break;
    case kVertexStage:
      CHECK(false) << "Vertex GpuProgram has its own ";
    default:
      CHECK(false) << "No such GpuProgram Type: " << (int32)stage;
      return NULL;
  }
  if (gpu_program->Init(this)) {
    return gpu_program.release();
  } else {
    return NULL;
  }
}

VertexGpuProgram* D3DRenderSystem::CreateVertexGpuProgram(
    VertexDescPtr desc, const std::string& program) {
  std::unique_ptr<VertexGpuProgram> gpu_program(
      new D3DVertexGpuProgram(desc, program));
  if (gpu_program->Init(this)) {
    return gpu_program.release();
  } else {
    return NULL;
  }
}

Technique* D3DRenderSystem::CreateTechnique() {
  return new D3DTechnique(this);
}

GpuConstantsTable* D3DRenderSystem::CreateGpuConstantsTable(
    int32 num, const GpuConstantsTable::Desc* desc) {
  std::unique_ptr<D3DGpuConstantsTable> tableptr(
      new D3DGpuConstantsTable(num, desc));
  if (tableptr->Init(this)) {
    return tableptr.release();
  } else {
    return NULL;
  }
}

Texture* D3DRenderSystem::CreateTexture(const Texture::Options& opt,
                                        const Image* img) {
  const ImageDataPtr& image = img->data(0);
  Texture::Options texopt = opt;
  texopt.width = image->width();
  texopt.height = image->height();
  texopt.format = image->format();
  texopt.type = (Texture::Type)img->type();
  std::unique_ptr<D3DTexture> tex;
  if (texopt.type == Texture::k2D) {
    tex.reset(new D3DTexture2D(texopt, this));
  } else if (texopt.type == Texture::kCubemap) {
    tex.reset(new D3DTextureCubeMap(texopt, this));
  } else {
    NOTREACHED();
    return NULL;
  }
  if (tex->InitFromImage(img)) {
    return tex.release();
  } else {
    return NULL;
  }
}

Texture* D3DRenderSystem::CreateTexture(const Texture::Options& opt) {
  if (opt.type == Texture::k2D) {
    return new D3DTexture2D(opt, this);
  } else {
    return NULL;
  }
}

/*
  RenderTarget* D3DRenderSystem::CreateRenderTarget(const Texture::Options& opt) {
  std::unique_ptr<D3DRenderTarget> target(new D3DRenderTarget(opt, false, this));
  if (target->Init(this)) {
  return target.release();
  } else {
  return NULL;
  }
  }

  DepthBuffer* D3DRenderSystem::CreateDepthBuffer(const Texture::Options& opt) {
  DCHECK(opt.format == kDepth24Stencil8);
  DCHECK(opt.target & Texture::kDepthStencil);
  std::unique_ptr<D3DDepthBuffer> depthbuffer(new D3DDepthBuffer(opt, this));
  if (depthbuffer->Init()) {
  return depthbuffer.release();
  } else {
  return false;
  }
  }
*/

Overlay* D3DRenderSystem::CreateOverlay(const gfx::RectF& rect) {
  std::unique_ptr<D3DOverlay> surface_ptr(new D3DOverlay(rect, this));
  if (surface_ptr->Init(this)) {
    return surface_ptr.release();
  } else {
    return NULL;
  }
}

Blending* D3DRenderSystem::CreateBlending(const Blending::Desc& desc) {
  std::unique_ptr<D3DBlending> blending(new D3DBlending(desc, this));
  if (blending->Init()) {
    return blending.release();
  } else {
    return NULL;
  }
}

Renderer* D3DRenderSystem::CreateRenderer(const Texture::Options& opt) {
  DCHECK(GetDefaultRenderer() != NULL);
  std::unique_ptr<D3DRenderer> renderer(new D3DRenderer(
      ((D3DRenderer*)GetDefaultRenderer())->GetContext(), this));
  if (renderer->Init(opt)) {
    return renderer.release();
  } else {
    return NULL;
  }
}

Renderer* D3DRenderSystem::CreateDeferredRenderer(const Texture::Options& opt) {
  DCHECK(GetDevice() != NULL);
  ID3D11DeviceContext* context = NULL;
  HRESULT hr = GetDevice()->CreateDeferredContext(0, &context);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to CreateDeferredContext";
    return NULL;
  }

  std::unique_ptr<D3DRenderer> renderer(new D3DRenderer(context, this));
  if (renderer->Init(opt)) {
    return renderer.release();
  } else {
    return NULL;
  }
}

bool D3DRenderSystem::InitD3DDevice() {
  return true;
}

Canvas2D* D3DRenderSystem::CreateCanvas2D() {
  std::unique_ptr<D3DCanvas2D> ptr(new D3DCanvas2D(GetContext2D()));
  if (ptr->Init()) {
    return ptr.release();
  } else {
    return NULL;
  }
}
}  // namespace d3d11
}  // namespace azer

