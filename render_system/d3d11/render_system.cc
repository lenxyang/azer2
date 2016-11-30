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
#include "azer/render_system/d3d11/render_target.h"
#include "azer/render_system/d3d11/rasterizer_state.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/sampler_state.h"
#include "azer/render_system/d3d11/shader.h"
#include "azer/render_system/d3d11/gpu_buffer.h"
#include "azer/render_system/d3d11/technique.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/texture_view.h"
#include "azer/render_system/d3d11/vertex_layout.h"

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

VertexLayoutPtr D3DRenderSystem::CreateVertexLayout(VertexDesc* desc) {
  VertexLayoutPtr layout = new D3DVertexLayout(desc);
  CHECK(layout->Init(this));
  return layout;
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

TexturePtr D3DRenderSystem::CreateTexture(const Texture::Options& opt,
                                          const ImageData* img) {
  Texture::Options texopt = opt;
  if (img) {
    if (texopt.size.width() == 0 && texopt.size.height() == 0) {
      texopt.size = gfx::Size(img->width(), img->height());
    }
    if (texopt.format != TexFormat::kUndefined) {
      texopt.format = (TexFormat)img->data_format();
    }
    if (texopt.type != TexType::kUnknown) {
      texopt.type = (TexType)img->textype();
    }
    CHECK_EQ(texopt.size.width(), img->width());
    CHECK_EQ(texopt.size.height(), img->height());
    CHECK_EQ(texopt.format, (TexFormat)img->data_format());
    CHECK_EQ(texopt.type, (TexType)img->textype());
  }
  scoped_refptr<Texture> tex;
  if (texopt.type == TexType::k2D) {
    tex = new D3DTexture2D(texopt, this);
  } else if (texopt.type == TexType::k2DArray) {
    tex = new D3DTexture2DArray(texopt, this);
  } else if (texopt.type == TexType::kCubemap) {
    tex = new D3DTextureCubeMap(texopt, this);
  } else if (texopt.type == TexType::k3D) {
    tex = new D3DTexture3D(texopt, this);
  } else {
    NOTREACHED();
    return TexturePtr();
  }
  bool ret = false;
  if (img) {
    ret = tex->InitFromImage(img);
  } else {
    ret = tex->Init();
  }

  return (ret ? tex : TexturePtr());
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

TextureViewPtr D3DRenderSystem::CreateTextureView(const TextureView::Options& opt, 
                                                  Texture* tex) {
  scoped_refptr<D3DResTextureView> ptr(new D3DResTextureView(opt, tex));
  if (ptr->Init(this)) {
    return ptr;
  } else {
    return TextureViewPtr();
  }
}

TextureUAViewPtr D3DRenderSystem::CreateTextureUAView(
    const TextureUAView::Options& opt, Texture* tex) {
  scoped_refptr<D3DTextureUAResView> ptr(new D3DTextureUAResView(opt, tex));
  if (ptr->Init(this)) {
    return ptr;
  } else {
    return TextureUAViewPtr();
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

GpuBufferPtr D3DRenderSystem::CreateBuffer(const GpuResOptions& opt, 
                                           int count, int strip,
                                           const uint8_t* data) {
  scoped_refptr<D3DGpuBuffer> buf(new D3DGpuBuffer(opt, count, strip));
  if (buf->Init(this, data)) {
    return buf;
  } else {
    return NULL;
  }
}

ShaderResViewPtr D3DRenderSystem::CreateBufferShaderResView(GpuBuffer* buffer) {
  scoped_refptr<D3DBufferResView> ptr(new D3DBufferResView(buffer));
  if (ptr->Init(this)) {
    return ptr;
  } else {
    return NULL;
  }
}

UnorderAccessViewPtr D3DRenderSystem::CreateBufferUAView(GpuBuffer* buf) {
  scoped_refptr<D3DBufferUAView> ptr(new D3DBufferUAView(buf));
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
  if (depth->Init((D3DTexture2D*)texture)) {
    return depth;
  } else {
    return DepthBufferPtr();
  }
}

RenderTargetPtr D3DRenderSystem::CreateRenderTarget(
    const RenderTarget::Options& opt, Texture* texture) {
  DCHECK(texture) << "Try Create DepthBuffer With None Texture";
  scoped_refptr<D3DRenderTarget> rt(new D3DRenderTarget(opt, false, this));
  if (rt->Init((D3DTexture2D*)texture)) {
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

