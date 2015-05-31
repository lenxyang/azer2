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
#include "azer/render_system/d3d11/reusable_object.h"
#include "azer/render_system/d3d11/overlay.h"
#include "azer/render_system/d3d11/technique.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/vertex_buffer.h"

namespace azer {
namespace d3d11 {

const StringType& D3DRenderSystem::name_ = AZER_LITERAL("Direct3D11RenderSystem");
const StringType& D3DRenderSystem::short_name_ = AZER_LITERAL("d3d11");

D3DRenderSystem::D3DRenderSystem(D3DEnvironmentPtr envptr)
    : envptr_(envptr) {
  InitContext2D();
}

D3DRenderSystem::~D3DRenderSystem() {
}

SwapChainPtr D3DRenderSystem::CreateSwapChainForSurface(Surface* surface) {
  scoped_refptr<D3DSwapChain> swapchain(new D3DSwapChain(this));
  if (swapchain->Init(surface)) {
    return swapchain;
  } else {
    return SwapChainPtr();
  }
}

bool D3DRenderSystem::Init() {
  std::unique_ptr<D3DReusableObject> ptr(new D3DReusableObject());
  if (ptr->Init(this)) {
    reusable_object_ = ptr.release();
    return true;
  } else {
    return false;
  }
}

bool D3DRenderSystem::reset() {
  return true;
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

VertexBufferPtr D3DRenderSystem::CreateVertexBuffer(
    const VertexBuffer::Options& opt, VertexData* dataptr) {
  scoped_refptr<D3DVertexBuffer> vertex_buffer(
      new D3DVertexBuffer(opt, this));
  if (vertex_buffer->Init(dataptr)) {
    return vertex_buffer;
  } else {
    return VertexBufferPtr();
  }
}


IndicesBufferPtr D3DRenderSystem::CreateIndicesBuffer(
    const IndicesBuffer::Options& opt, IndicesData* dataptr) {
  scoped_refptr<D3DIndicesBuffer> indices_buffer(
      new D3DIndicesBuffer(opt, this));
  if (indices_buffer->Init(dataptr)) {
    return indices_buffer;
  } else {
    return IndicesBufferPtr();
  }
}

GpuProgramPtr D3DRenderSystem::CreateGpuProgram(RenderPipelineStage stage,
                                              const std::string& program) {
  GpuProgramPtr gpu_program;
  switch (stage) {
    case kPixelStage:
      gpu_program = (new D3DPixelGpuProgram(program));
      break;
    case kGeometryStage:
      gpu_program = (new D3DGeometryGpuProgram(program));
      break;
    case kVertexStage:
      CHECK(false) << "Vertex GpuProgram has its own ";
    default:
      CHECK(false) << "No such GpuProgram Type: " << (int32)stage;
      return NULL;
  }
  if (gpu_program->Init(this)) {
    return gpu_program;
  } else {
    return GpuProgramPtr();
  }
}

VertexGpuProgramPtr D3DRenderSystem::CreateVertexGpuProgram(
    VertexDescPtr desc, const std::string& program) {
  VertexGpuProgramPtr gpu_program(new D3DVertexGpuProgram(desc, program));
  if (gpu_program->Init(this)) {
    return gpu_program;
  } else {
    return VertexGpuProgramPtr();
  }
}

TechniquePtr D3DRenderSystem::CreateTechnique() {
  TechniquePtr ptr (new D3DTechnique(this));
  return ptr;
}

GpuConstantsTablePtr D3DRenderSystem::CreateGpuConstantsTable(
    int32 num, const GpuConstantsTable::Desc* desc) {
  scoped_refptr<D3DGpuConstantsTable> tableptr = new D3DGpuConstantsTable(num, desc);
  if (tableptr->Init(this)) {
    return tableptr;
  } else {
    return GpuConstantsTablePtr();
  }
}

TexturePtr D3DRenderSystem::CreateTexture(const Texture::Options& opt,
                                        const Image* img) {
  const ImageDataPtr& image = img->data(0);
  Texture::Options texopt = opt;
  texopt.size = gfx::Size(image->width(), image->height());
  texopt.format = image->format();
  texopt.type = (Texture::Type)img->type();
  scoped_refptr<D3DTexture> tex;
  if (texopt.type == Texture::k2D) {
    tex = new D3DTexture2D(texopt, this);
  } else if (texopt.type == Texture::kCubemap) {
    tex = new D3DTextureCubeMap(texopt, this);
  } else {
    NOTREACHED();
    return TexturePtr();
  }
  if (tex->InitFromImage(img)) {
    return tex;
  } else {
    return TexturePtr();
  }
}

TexturePtr D3DRenderSystem::CreateTexture(const Texture::Options& opt) {
  if (opt.type == Texture::k2D) {
    scoped_refptr<D3DTexture2D> ptr(new D3DTexture2D(opt, this));
    if (ptr->Init(NULL, 1)) {
      return ptr;
    } else {
      return TexturePtr();
    }
  } else {
    return TexturePtr();
  }
}

OverlayPtr D3DRenderSystem::CreateOverlay() {
  scoped_refptr<D3DOverlay> surface_ptr(new D3DOverlay(this));
  if (surface_ptr->Init(this)) {
    return surface_ptr;
  } else {
    return OverlayPtr();
  }
}

BlendingPtr D3DRenderSystem::CreateBlending(const Blending::Desc& desc) {
  scoped_refptr<D3DBlending> blending(new D3DBlending(desc, this));
  if (blending->Init()) {
    return blending;
  } else {
    return BlendingPtr();
  }
}

RendererPtr D3DRenderSystem::CreateRenderer(const Texture::Options& opt) {
  DCHECK(envptr_.get() != NULL);
  DCHECK(envptr_->GetContext() != NULL);
  ID3D11DeviceContext* context = envptr_->GetContext();
  scoped_refptr<D3DRenderer> renderer(new D3DRenderer(context, this));
  if (renderer->Init(opt)) {
    return renderer;
  } else {
    return RendererPtr();
  }
}

RendererPtr D3DRenderSystem::CreateDeferredRenderer(const Texture::Options& opt) {
  DCHECK(GetDevice() != NULL);
  ID3D11DeviceContext* context = NULL;
  HRESULT hr = GetDevice()->CreateDeferredContext(0, &context);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to CreateDeferredContext";
    return NULL;
  }

  scoped_refptr<D3DRenderer> renderer(new D3DRenderer(context, this));
  if (renderer->Init(opt)) {
    return renderer;
  } else {
    return RendererPtr();
  }
}

}  // namespace d3d11
}  // namespace azer

