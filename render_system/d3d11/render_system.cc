#include "azer/render_system/d3d11/render_system.h"

#include <string>


#include "base/strings/string16.h"
#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/base/image.h"
#include "azer/render/common.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/blending.h"
#include "azer/render_system/d3d11/depth_buffer.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_compute_task.h"
#include "azer/render_system/d3d11/gpu_constants_table.h"
#include "azer/render_system/d3d11/indices_buffer.h"
#include "azer/render_system/d3d11/render_target.h"
#include "azer/render_system/d3d11/rasterizer_state.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/sampler_state.h"
#include "azer/render_system/d3d11/shader.h"
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
    const HBufferOptions& opt, SlotVertexData* dataptr) {
  scoped_refptr<D3DVertexBuffer> vertex_buffer(
      new D3DVertexBuffer(opt, this));
  if (vertex_buffer->Init(dataptr)) {
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
    const HBufferOptions& opt, IndicesData* dataptr) {
  scoped_refptr<D3DIndicesBuffer> indices_buffer(
      new D3DIndicesBuffer(opt, this));
  if (indices_buffer->Init(dataptr)) {
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
      CHECK(false) << "No such Shader Type: " << (int32)info.stage;
      return NULL;
  }

  if (gpu_program->Init(this)) {
    return gpu_program;
  } else {
    return ShaderPtr();
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
                                          const ImageData* img) {
  Texture::Options texopt = opt;
  texopt.size = gfx::Size(img->width(), img->height());
  texopt.format = (TexFormat)img->data_format();
  texopt.type = (TexType)img->textype();
  scoped_refptr<D3DTexture> tex;
  if (texopt.type == kTex2D) {
    tex = new D3DTexture2D(texopt, this);
  } else if (texopt.type == kTex2DArray) {
    tex = new D3DTexture2DArray(texopt, this);
  } else if (texopt.type == kTexCubemap) {
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
  if (opt.type == kTex2D) {
    scoped_refptr<D3DTexture2D> ptr(new D3DTexture2D(opt, this));
    if (ptr->Init(NULL, 1, opt.mipmap_level)) {
      return ptr;
    } else {
      return TexturePtr();
    }
  } else {
    return TexturePtr();
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

GpuComputeTaskDispatcherPtr D3DRenderSystem::CreateDispatcher() {
  GpuComputeTaskDispatcherPtr ptr(new D3DGpuComputeTaskDispatcher);
  return ptr;
}

BlendingPtr D3DRenderSystem::CreateBlending(const Blending::BlendDesc& desc) {
  scoped_refptr<D3DBlending> blending(new D3DBlending(desc, this));
  if (blending->Init()) {
    return blending;
  } else {
    return BlendingPtr();
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
  DCHECK(texture->options().target & kBindTargetDepthStencil);
  scoped_refptr<D3DDepthBuffer> depthbuffer(new D3DDepthBuffer(opt, texture));
  if (depthbuffer.Init((D3DTexture*)texture)) {
    return depthbuffer;
  } else {
    return DepthBufferPtr();
  }
}

RenderTargetPtr D3DRenderSystem::CreateRenderTarget(
    const RenderTarget::Options& opt, Texture* texture) {
  return RenderTargetPtr();
}

RendererPtr D3DRenderSystem::CreateRenderer(const Texture::Options& opt,
                                            const Texture::Options& depthopt) {
  DCHECK(envptr_.get() != NULL);
  DCHECK(envptr_->GetContext() != NULL);
  DCHECK(opt.size == depthopt.size);
  ID3D11DeviceContext* context = envptr_->GetContext();
  scoped_refptr<D3DRenderer> renderer(new D3DRenderer(context, this));
  if (renderer->Init(opt)) {
    return renderer;
  } else {
    return RendererPtr();
  }
}

RendererPtr D3DRenderSystem::CreateMultipleOutputRenderer(
    int count, const Texture::Options* opts, DepthBuffer* depth) {
  DCHECK_GE(count, 0u);
  std::vector<RenderTargetPtr> targets;
  for (int32 i = 0; i < count; ++i) {
    DCHECK(opts[i].size == depth->size());
    scoped_refptr<D3DRenderTarget> target = D3DRenderTarget::Create(opts[i], this);
    targets.push_back(target);
  }

  ID3D11DeviceContext* context = envptr_->GetContext();
  scoped_refptr<D3DRenderer> renderer(new D3DRenderer(context, this));
  if (renderer->Init(&targets, depth)) {
    return renderer;
  } else {
    return RendererPtr();
  }
}

RendererPtr D3DRenderSystem::CreateMultipleOutputRenderer(
    int32 count, const Texture::Options* opts,
    const Texture::Options& depthopt) {
  DepthBufferPtr depth(D3DDepthBuffer::Create(depthopt, this));
  CHECK(depth.get()) << "Failed to Create DepthBuffer";
  return CreateMultipleOutputRenderer(count, opts, depth);
}
}  // namespace d3d11
}  // namespace azer

