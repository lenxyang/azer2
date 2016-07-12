
#include "azer/render_system/d3d11/depth_buffer.h"

#include "azer/render/surface.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
namespace {
ID3D11Device* GetDevice() {
  RenderSystem* rs  = RenderSystem::Current();
  D3DRenderSystem* d3drs = dynamic_cast<D3DRenderSystem*>(rs);
  DCHECK(d3drs);
  return d3drs->GetDevice();
}
}

// class D3DDepthStencilState
D3DDepthStencilState::D3DDepthStencilState() {
  HRESULT hr;
  D3D11_DEPTH_STENCIL_DESC depth_desc;
  ZeroMemory(&depth_desc, sizeof(depth_desc));
  depth_desc.DepthEnable = TRUE;
  depth_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depth_desc.DepthFunc = D3D11_COMPARISON_LESS;
  depth_desc.StencilEnable = FALSE;
  depth_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
  depth_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
  depth_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  depth_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  depth_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depth_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depth_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
  depth_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
  depth_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

  hr = GetDevice()->CreateDepthStencilState(&depth_desc, &depth_state_);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateDepthStencilState failed");
}

D3DDepthStencilState::~D3DDepthStencilState() {
  SAFE_RELEASE(depth_state_);
}

void D3DDepthStencilState::SetDepthStencilState(
    const D3D11_DEPTH_STENCIL_DESC& desc) {
  DCHECK(depth_state_);
  SAFE_RELEASE(depth_state_);
  HRESULT hr = GetDevice()->CreateDepthStencilState(&desc, &depth_state_);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateDepthStencilState failed ");
}

void D3DDepthStencilState::EnableDepthTest(bool enable) {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  desc.DepthEnable = enable;
  return SetDepthStencilState(desc);
}

bool D3DDepthStencilState::IsDepthTestEnabled() {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  return desc.DepthEnable == TRUE;
}

void D3DDepthStencilState::SetDepthWriteMask(uint32_t mask) {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  desc.DepthWriteMask = (mask == 0 ? D3D11_DEPTH_WRITE_MASK_ZERO:
                         D3D11_DEPTH_WRITE_MASK_ALL);
  SetDepthStencilState(desc);
}

void D3DDepthStencilState::SetDepthCompareFunc(CompareFunc func) {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  desc.DepthFunc = TranslateCompareFunc(func);
  SetDepthStencilState(desc);
}

void D3DDepthStencilState::EnableStencil(bool enable) {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  desc.StencilEnable = enable;
  SetDepthStencilState(desc);
}

bool D3DDepthStencilState::IsStencilTestEnabled() {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  return desc.StencilEnable == TRUE;
}

void D3DDepthStencilState::SetStencilMask(uint8_t read_mask, uint8_t write_mask) {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  desc.StencilReadMask = read_mask;
  desc.StencilWriteMask = write_mask;
  SetDepthStencilState(desc);
}

void D3DDepthStencilState::SetFrontFaceOper(const StencilOperStruct& oper) {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  desc.FrontFace.StencilFunc = TranslateCompareFunc(oper.stencil_func);
  desc.FrontFace.StencilDepthFailOp = TranslateStencilOper(oper.depth_failed_op);
  desc.FrontFace.StencilPassOp = TranslateStencilOper(oper.pass_op);
  desc.FrontFace.StencilFailOp = TranslateStencilOper(oper.failed_op);
  SetDepthStencilState(desc);
}

void D3DDepthStencilState::SetBackFaceOper(const StencilOperStruct& oper) {
  D3D11_DEPTH_STENCIL_DESC desc;  
  depth_state_->GetDesc(&desc);
  desc.BackFace.StencilFunc = TranslateCompareFunc(oper.stencil_func);
  desc.BackFace.StencilDepthFailOp = TranslateStencilOper(oper.depth_failed_op);
  desc.BackFace.StencilPassOp = TranslateStencilOper(oper.pass_op);
  desc.BackFace.StencilFailOp = TranslateStencilOper(oper.failed_op);
  SetDepthStencilState(desc);
}

void D3DDepthStencilState::Apply(Renderer* r, uint32_t stencilref) {
  D3DRenderer* renderer = dynamic_cast<D3DRenderer*>(r);
  renderer->GetContext()->OMSetDepthStencilState(depth_state_, stencilref);
}

// class D3DDepthBuffer 
D3DDepthBuffer::D3DDepthBuffer(const Options& opt, D3DRenderSystem* rs)
    : DepthBuffer(opt),
      target_(NULL),
      texres_(NULL),
      render_system_(rs) {
}

D3DDepthBuffer::~D3DDepthBuffer() {
  SAFE_RELEASE(target_);
  SAFE_RELEASE(texres_);
}


bool D3DDepthBuffer::Init(D3DTexture* tex) {
  DCHECK(!texture_.get());
  DCHECK(tex->options().target & kBindTargetDepthStencil);
  texture_ = tex;
  ID3D11Device* d3d_device = render_system_->GetDevice();
  HRESULT hr;
  texres_ = tex->GetResource();
  texres_->AddRef();
  D3D11_DEPTH_STENCIL_VIEW_DESC dvsd;
  memset(&dvsd, 0, sizeof(dvsd));
  if (options().depth_readonly) {
    dvsd.Flags |= D3D11_DSV_READ_ONLY_DEPTH;
  }
  if (options().stencil_readonly) {
    dvsd.Flags |= D3D11_DSV_READ_ONLY_STENCIL;
  }

  TexType type = (options().type != TexType::kUnknown) ?
      options().type : tex->options().type;
  TexFormat format = (options().format != TexFormat::kUndefined) ?
      options().format : tex->options().format;
  dvsd.Format = TranslateTexFormat(format);
  bool multisampler = (tex->options().sample_desc.count > 1);
  if (type == TexType::k2D) {
    dvsd.ViewDimension = (!multisampler) ? D3D11_DSV_DIMENSION_TEXTURE2D
        : D3D11_DSV_DIMENSION_TEXTURE2DMS;
  } else if (type == TexType::kCubemap) {
	  dvsd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	  dvsd.Texture2DArray.MipSlice = 0;
	  dvsd.Texture2DArray.FirstArraySlice = 0;
	  dvsd.Texture2DArray.ArraySize = 1;
	  CHECK_GT(tex->options().diminison, 0);
  } else if (type == TexType::k2DArray) {
    dvsd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    dvsd.Texture2DArray.MipSlice = 0;
    dvsd.Texture2DArray.FirstArraySlice = 0;
    // dvsd.Texture2DArray.ArraySize = tex->options().diminison;
    dvsd.Texture2DArray.ArraySize = 1;
    CHECK_GT(tex->options().diminison, 0);
  } else {
    CHECK(false) << "Unsupport TexType[" << type << " for depth";
  }
  if (tex->options().format == TexFormat::kR24UNormG8Uint) {
    CHECK(format == TexFormat::kD24UNormS8Uint);
  }

  hr = d3d_device->CreateDepthStencilView(texres_, &dvsd, &target_);
  HRESULT_HANDLE(hr, ERROR, "CreateDepthStencilView failed ");

  return true;
}

void D3DDepthBuffer::Clear(D3DRenderer* renderer, bool clear_depth,
                           bool clear_stencil, float depth_val, int stencil_val) {
  DCHECK(target_ != NULL);
  ID3D11DeviceContext* d3d_context = renderer->GetContext();
  uint32_t flags = 0;
  flags |= (clear_depth ? D3D11_CLEAR_DEPTH : 0);
  flags |= (clear_stencil ? D3D11_CLEAR_STENCIL : 0);
  d3d_context->ClearDepthStencilView(target_,
                                     flags,
                                     depth_val, // depth value (clamp(0.0, 1.0)
                                     stencil_val);   // stencil value
}

NativeGpuResourceViewHandle D3DDepthBuffer::native_handle() {
  CHECK(false);
  return (NativeGpuResourceViewHandle)target_;
}
}  // namespace d3d11
}  // namespace azer
