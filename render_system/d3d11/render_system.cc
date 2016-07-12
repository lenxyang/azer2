#include "azer/render_system/d3d11/render_system.h"

#include <string>


#include "base/strings/string16.h"

#include "base/logging.h"
#include "azer/base/image.h"
#include "azer/render/common.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/blending.h"
#include "azer/render_system/d3d11/depth_buffer.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_constants_table.h"
#include "azer/render_system/d3d11/indices_buffer.h"
#include "azer/render_system/d3d11/render_target.h"
#include "azer/render_system/d3d11/rasterizer_state.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/sampler_state.h"
#include "azer/render_system/d3d11/shader.h"
#include "azer/render_system/d3d11/structured_buffer.h"
#include "azer/render_system/d3d11/technique.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/texture_view.h"
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
  GetDriverCapability();
  return true;
}

bool D3DRenderSystem::reset() {
  return true;
}

void D3DRenderSystem::GetDriverCapability() {
  capability_.hw_transform_light_ = true;

  // get sample desc
  envptr_->GetD3DMultisampleSupported(&capability_.sample_desc_);
}

const StringType& D3DRenderSystem::name() const {
  return name_;
}

const StringType& D3DRenderSystem::short_name() const {
  return short_name_;
}

VertexBufferPtr D3DRenderSystem::CreateVertexBuffer(
    const GpuBufferOptions& opt, SlotVertexData* dataptr) {
  scoped_refptr<D3DVertexBuffer> vertex_buffer(new D3DVertexBuffer(opt));
  if (vertex_buffer->Init(dataptr, this)) {
    return vertex_buffer;
  } else {
    return VertexBufferPtr();
  }
}

VertexBufferGroupPtr D3DRenderSystem::CreateVertexBufferGroup(VertexDesc* desc) {
  scoped_refptr<D3DVertexBufferGroup> ptr(new D3DVertexBufferGroup(desc, this));
  return ptr;
}

IndicesBufferPtr D3DRenderSystem::CreateIndicesBuffer(
    const GpuBufferOptions& opt, IndicesData* dataptr) {
  scoped_refptr<D3DIndicesBuffer> indices_buffer(new D3DIndicesBuffer(opt));
  if (indices_buffer->Init(dataptr, this)) {
    return indices_buffer;
  } else {
    return IndicesBufferPtr();
  }
}

ShaderPtr D3DRenderSystem::CreateShader(const ShaderInfo& info, VertexDesc* desc) {
  scoped_refptr<D3DShader> gpu_program;
  switch (info.stage) {
    case kPixelStage:
      DCHECK(NULL == desc);
      gpu_program = (new D3DPixelShader(info));
      break;
    case kGeometryStage:
      gpu_program = (new D3DGeometryShader(desc, info));
      break;
    case kVertexStage:
      gpu_program = new D3DVertexShader(desc, info);
      break;
    case kHullStage:
      DCHECK(NULL == desc);
      gpu_program = new D3DHullShader(info);
      break;
    case kDomainStage:
      DCHECK(NULL == desc);
      gpu_program = new D3DDomainShader(info);
      break;
    case kComputeStage:
      DCHECK(NULL == desc);
      gpu_program = new D3DComputeShader(info);
      break;
    default:
      CHECK(false) << "No such Shader Type: " << (int32_t)info.stage;
      return NULL;
  }

  if (gpu_program->Init(this)) {
    return gpu_program;
  } else {
    return ShaderPtr();
  }
}

TechniquePtr D3DRenderSystem::CreateTechnique() {
  TechniquePtr ptr (new D3DTechnique());
  return ptr;
}

GpuConstantsTablePtr D3DRenderSystem::CreateGpuConstantsTable(
    int32_t num, const GpuConstantsTable::Desc* desc) {
  scoped_refptr<D3DGpuConstantsTable> tableptr = new D3DGpuConstantsTable(num, desc);
  if (tableptr->Init(this)) {
    return tableptr;
  } else {
    return GpuConstantsTablePtr();
  }
}

TexturePtr D3DRenderSystem::CreateTexture(const Texture::Options& opt,
                                          const ImageData* img) {
  Texture::Options texopt = opt;
  texopt.size = gfx::Size(img->width(), img->height());
  texopt.format = (TexFormat)img->data_format();
  texopt.type = (TexType)img->textype();
  scoped_refptr<D3DTexture> tex;
  if (texopt.type == TexType::k2D) {
    tex = new D3DTexture2D(texopt, this);
  } else if (texopt.type == TexType::k2DArray) {
    tex = new D3DTexture2DArray(texopt, this);
  } else if (texopt.type == TexType::kCubemap) {
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
  scoped_refptr<D3DTexture> ptr;
  if (opt.type == TexType::k2D) {
    ptr = new D3DTexture2D(opt, this);
  } else if (opt.type == TexType::kCubemap) {
    ptr = new D3DTextureCubeMap(opt, this);
  } else if (opt.type == TexType::k2DArray) {
    ptr = new D3DTexture2DArray(opt, this);
  } else {
    CHECK(false) << "Unsupport Texture Type: " << opt.type;
    return TexturePtr();
  }

  if (ptr->Init(NULL)) {
    return ptr;
  } else {
    return TexturePtr();
  }
}

TextureViewPtr D3DRenderSystem::CreateTextureView(const TextureView::Options& opt, 
                                                  Texture* tex) {
  scoped_refptr<D3DTextureView> ptr(new D3DResTextureView(opt, tex));
  if (ptr->Init()) {
    return ptr;
  } else {
    return TextureViewPtr();
  }
}

SamplerStatePtr D3DRenderSystem::CreateSamplerState(
    const SamplerState::Options& opt) {
  scoped_refptr<D3DSamplerState> ptr(new D3DSamplerState(opt, this));
  if (ptr->Init()) {
    return ptr;
  } else {
    return SamplerStatePtr();
  }
}

BlendingPtr D3DRenderSystem::CreateBlending(const Blending::BlendDesc& desc) {
  scoped_refptr<D3DBlending> blending(new D3DBlending(desc, this));
  if (blending->Init()) {
    return blending;
  } else {
    return BlendingPtr();
  }
}

StructuredGpuBufferPtr D3DRenderSystem::CreateStructuredBuffer(
    const GpuBufferOptions& opt, int count, int strip) {
  scoped_refptr<D3DStructuredGpuBuffer> buf(new D3DStructuredGpuBuffer(
      opt, count, strip));
  if (buf->Init(this)) {
    return buf;
  } else {
    return NULL;
  }
}

ShaderResViewPtr D3DRenderSystem::CreateShaderResView(
    const ShaderResView::Options& opt, GpuBuffer* buffer) {
}

UnorderAccessResViewPtr D3DRenderSystem::CreateUnorderAccessResView(
    const UnorderAccessResView::Options& opt, GpuBuffer* buffer) {
}
StructuredGpuBufferViewPtr D3DRenderSystem::CreateStructuredBufferView(
    const StructuredGpuBufferView::Options& opt, StructuredGpuBuffer* buffer) {
  scoped_refptr<D3DUAStructuredGpuBufferView> ptr(
      new D3DUAStructuredGpuBufferView(opt, buffer));
  if (ptr->Init(this)) {
    return ptr;
  } else {
    return NULL;
  }
}

RasterizerStatePtr D3DRenderSystem::CreateRasterizerState() {
  return RasterizerStatePtr(new D3DRasterizerState);
}

DepthStencilStatePtr D3DRenderSystem::CreateDepthStencilState() {
  return DepthStencilStatePtr(new D3DDepthStencilState);
}

DepthBufferPtr D3DRenderSystem::CreateDepthBuffer(const DepthBuffer::Options& opt, 
                                                  Texture* texture) {
  DCHECK(texture) << "Try Create DepthBuffer With None Texture";
  DCHECK(texture->options().target & kBindTargetDepthStencil);
  scoped_refptr<D3DDepthBuffer> depth(new D3DDepthBuffer(opt, this));
  if (depth->Init((D3DTexture*)texture)) {
    return depth;
  } else {
    return DepthBufferPtr();
  }
}

RenderTargetPtr D3DRenderSystem::CreateRenderTarget(
    const RenderTarget::Options& opt, Texture* texture) {
  DCHECK(texture) << "Try Create DepthBuffer With None Texture";
  scoped_refptr<D3DRenderTarget> rt(new D3DRenderTarget(opt, false, this));
  if (rt->Init((D3DTexture*)texture)) {
    return rt;
  } else {
    return RenderTargetPtr();
  }
}

RendererPtr D3DRenderSystem::CreateRenderer(RenderTargetPtrs* targets, 
                                            DepthBuffer* depth) {
  ID3D11DeviceContext* context = envptr_->GetContext();
  scoped_refptr<D3DRenderer> renderer(new D3DRenderer(context, this));
  if (renderer->Init(targets, depth)) {
    return renderer;
  } else {
    return RendererPtr();
  }
}

}  // namespace d3d11
}  // namespace azer

