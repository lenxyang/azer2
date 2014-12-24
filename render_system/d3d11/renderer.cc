#include "azer/render_system/d3d11/renderer.h"

#include "base/strings/string16.h"
#include "base/basictypes.h"
#include "base/logging.h"

#include "azer/render/surface.h"
#include "azer/render/render_system.h"
#include "azer/render_system/d3d11/blending.h"
#include "azer/render_system/d3d11/depth_buffer.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_constants_table.h"
#include "azer/render_system/d3d11/indices_buffer.h"
#include "azer/render_system/d3d11/render_target.h"
#include "azer/render_system/d3d11/technique.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/vertex_buffer.h"
#include "azer/render_system/d3d11/util.h"

namespace azer {

#define GET_D3D11_RENDER_STATE()                        \
  DCHECK(d3d_context_ != NULL);                         \
  ID3D11RasterizerState* obj = NULL;                    \
  d3d_context_->RSGetState(&obj);                       \
  D3DObjPtr setting(obj);                               \
  D3D11_RASTERIZER_DESC desc;                           \
  ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));     \
  obj->GetDesc(&desc);

#define SET_D3D11_RENDER_STATE()  {                                     \
  ID3D11RasterizerState* newobj = NULL;                                 \
  HRESULT hr = GetDevice()->CreateRasterizerState(&desc, &newobj);      \
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateTasterizerState failed ");     \
  D3DObjPtr auto_ptr(newobj);                                           \
  d3d_context_->RSSetState(newobj);                                     \
}

const std::string& D3D11Renderer::name_ = "Direct3D11Renderer";

const std::string& D3D11Renderer::name() const {
  return name_;
}

D3D11Renderer::D3D11Renderer(ID3D11DeviceContext* context, D3D11RenderSystem* rs)
    : Renderer(1, rs)
    , d3d11_render_system_(rs)
    , d3d_context_(context) {
}

D3D11Renderer::~D3D11Renderer() {
  SAFE_RELEASE(d3d_context_);
}

void D3D11Renderer::Use() {
  DCHECK(!targets_.empty() && targets_[0].get() != NULL);
  DCHECK(depth_.get() != NULL);
  ID3D11RenderTargetView* target_view[1] = {0};
  target_view[0] =
      ((D3D11RenderTarget*)targets_[0].get())->GetD3D11RenderTargetView();
  ID3D11DepthStencilView* depth_view =
      ((D3D11DepthBuffer*)depth_.get())->GetD3D11DepthStencilView();
  d3d_context_->OMSetRenderTargets(1, target_view, depth_view);
}

void D3D11Renderer::Reset() {
  DCHECK(d3d_context_ != NULL);
  d3d_context_->ClearState();
  InitRenderState();
  SetFrontFace(azer::kCounterClockwise);
  SetCullingMode(azer::kCullBack);
  EnableDepthTest(false);
}

bool D3D11Renderer::IsDepthTestEnable() {
  return depth_ && depth_->IsEnabled();
}

void D3D11Renderer::EnableDepthTest(bool enable) {
  CHECK(depth_);
  depth_->Enable(enable);
}

FrontFace D3D11Renderer::GetFrontFace(void) {
  GET_D3D11_RENDER_STATE();
  if (desc.FrontCounterClockwise) return kCounterClockwise;
  else return kClockwise;
}

void D3D11Renderer::SetFrontFace(FrontFace mode) {
  GET_D3D11_RENDER_STATE();
  desc.FrontCounterClockwise = (mode == kCounterClockwise);
  SET_D3D11_RENDER_STATE();
}

FillMode D3D11Renderer::GetFillMode(void) {
  GET_D3D11_RENDER_STATE();
  return TranslateD3DFillMode(desc.FillMode);
}

void D3D11Renderer::SetFillMode(FillMode mode) {
  GET_D3D11_RENDER_STATE();
  desc.FillMode = TranslateFillMode(mode);
  SET_D3D11_RENDER_STATE();
}

CullingMode D3D11Renderer::GetCullingMode(void) {
  GET_D3D11_RENDER_STATE();
  return TranslateD3DCullingMode(desc.CullMode);
}

void D3D11Renderer::SetCullingMode(CullingMode mode) {
  GET_D3D11_RENDER_STATE();
  desc.CullMode = TranslateCullingMode(mode);
  SET_D3D11_RENDER_STATE();
}

void D3D11Renderer::EnableMultisampleAntiAliasing(bool enable) {
  GET_D3D11_RENDER_STATE();
  desc.MultisampleEnable = (enable ? TRUE : FALSE);
  SET_D3D11_RENDER_STATE();
}

bool D3D11Renderer::IsMultisampleAntiAliasingEnabled() {
  GET_D3D11_RENDER_STATE();
  return desc.MultisampleEnable == TRUE;
} 

void D3D11Renderer::EnableLineAntialiasing(bool enable) {
  GET_D3D11_RENDER_STATE();
  desc.AntialiasedLineEnable = (enable ? TRUE : FALSE);
  SET_D3D11_RENDER_STATE();
}

bool D3D11Renderer::IsLineAntialiasingEnabled() {
  GET_D3D11_RENDER_STATE();
  return desc.AntialiasedLineEnable == TRUE;
}

void D3D11Renderer::ResetBlending() {
  d3d_context_->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void D3D11Renderer::UseBlending(Blending* vblending, float* factor, uint32 mask) {
  DCHECK(NULL != d3d_context_);
  D3D11Blending* blending = (D3D11Blending*)vblending;
  DCHECK(NULL != blending->blending_state_);
  d3d_context_->OMSetBlendState(blending->blending_state_, factor, mask);
}

void D3D11Renderer::Clear(const azer::Vector4& color) {
  DCHECK(d3d_context_ != NULL);
  DCHECK(targets_[0].get() != NULL);
  ID3D11RenderTargetView* target_view =
      ((D3D11RenderTarget*)targets_[0].get())->GetD3D11RenderTargetView();
  DCHECK(target_view != NULL);
  d3d_context_->ClearRenderTargetView(
      target_view,
      D3DXCOLOR(color.x, color.y, color.z, color.w));
}

void D3D11Renderer::ClearDepthAndStencil(DepthBuffer::ClearFlag flag,
                                         float depth_val, int stencil_val) {
  DCHECK(NULL != depth_.get());
  ((D3D11DepthBuffer*)depth_.get())->Clear(this, flag, depth_val, stencil_val);
}

void D3D11Renderer::ResetShader(RenderPipelineStage stage) {
  switch (stage) {
    case kVertexStage:
      d3d_context_->VSSetShader(NULL, 0, 0);
      break;
    case kPixelStage:
      d3d_context_->PSSetShader(NULL, 0, 0);
      break;
    case kGeometryStage:
      d3d_context_->GSSetShader(NULL, 0, 0);
      break;
    default:
      CHECK(false) << "No such GpuProgram Type: " << (int32)stage;
  }
}

void D3D11Renderer::Draw(VertexBuffer* vvb, PrimitiveTopology primitive,
                         int vertices_num, int32 start_vertex) {
  const int num_of_vertices =
      (vertices_num != -1) ? vertices_num : vvb->vertex_num();
  CHECK(num_of_vertices > 0);
  D3D11VertexBuffer* vb = (D3D11VertexBuffer*)vvb;
  DCHECK(vb->Initialized()) << "VertexBuffer not initialized.";
  UINT stride = vvb->element_size();
  UINT offset = 0;
  d3d_context_->IASetVertexBuffers(0, 1, &vb->buffer_, &stride, &offset);
  d3d_context_->IASetPrimitiveTopology(
      TranslatePrimitiveTopology(primitive));
  d3d_context_->Draw(num_of_vertices, start_vertex);
}

void D3D11Renderer::DrawIndex(VertexBuffer* vvb, IndicesBuffer* vib,
                              PrimitiveTopology primitive, int indices_num,
                              int32 first_indices, int32 index_base) {
  const int num_of_indices =
      (indices_num != -1) ? indices_num : vib->indices_num();
  UINT stride = vvb->element_size();
  UINT offset = 0;
  D3D11VertexBuffer* vb = (D3D11VertexBuffer*)vvb;
  DCHECK(NULL != vb && vb->Initialized()) << "VertexBuffer not initialized.";
  D3D11IndicesBuffer* ib = (D3D11IndicesBuffer*)vib;
  DCHECK(NULL != ib && ib->Initialized()) << "IndicesBuffer not initialized.";
  d3d_context_->IASetVertexBuffers(0, 1, &vb->buffer_, &stride, &offset);
  d3d_context_->IASetIndexBuffer(ib->buffer_,
                                 TranslateIndexType(ib->type()),
                                 0);
  d3d_context_->IASetPrimitiveTopology(TranslatePrimitiveTopology(primitive));
  d3d_context_->DrawIndexed(num_of_indices, first_indices, index_base);
}

void D3D11Renderer::DrawInstanced(int32 instance_num, VertexBuffer* vvb,
                                  PrimitiveTopology primitive,
                                  int32 vertices_num, int32 first_vertex,
                                  int32 instance_start_index) {
  D3D11VertexBuffer* vb = (D3D11VertexBuffer*)vvb;
  DCHECK(NULL != vb && vb->Initialized()) << "VertexBuffer not initialized.";
  const int num_of_vertices =
      (vertices_num != -1) ? vertices_num : vb->vertex_num();
  UINT stride = vvb->element_size();
  UINT offset = 0;
  d3d_context_->IASetVertexBuffers(0, 1, &vb->buffer_, &stride, &offset);
  d3d_context_->IASetPrimitiveTopology(TranslatePrimitiveTopology(primitive));
  d3d_context_->DrawInstanced(num_of_vertices, instance_num,
                              first_vertex, instance_start_index);
}

void D3D11Renderer::DrawIndexInstanced(int32 instance_num, VertexBuffer* vvb,
                                       IndicesBuffer* vib,
                                       PrimitiveTopology primitive,
                                       int32 indices_num, int32 first_indices,
                                       int32 index_base,
                                       int32 instance_start_index) {
  const int num_of_indices =
      (indices_num != -1) ? indices_num : vib->indices_num();
  UINT stride = vvb->element_size();
  UINT offset = 0;
  D3D11VertexBuffer* vb = (D3D11VertexBuffer*)vvb;
  DCHECK(NULL != vb && vb->Initialized()) << "VertexBuffer not initialized.";
  D3D11IndicesBuffer* ib = (D3D11IndicesBuffer*)vib;
  DCHECK(NULL != ib && ib->Initialized()) << "IndicesBuffer not initialized.";
  d3d_context_->IASetVertexBuffers(0, 1, &vb->buffer_, &stride, &offset);
  d3d_context_->IASetIndexBuffer(ib->buffer_,
                                 TranslateIndexType(ib->type()),
                                 0);
  d3d_context_->IASetPrimitiveTopology(TranslatePrimitiveTopology(primitive));
  d3d_context_->DrawIndexedInstanced(num_of_indices, instance_num,
                                     first_indices, index_base,
                                     instance_start_index);
}

void D3D11Renderer::UseConstantsTable(RenderPipelineStage stage,
                                          GpuConstantsTable* table) {
  D3D11GpuConstantsTable* constants = (D3D11GpuConstantsTable*)table;
  if (stage == kVertexStage) {
    d3d_context_->VSSetConstantBuffers(0, 1, &(constants->buffer_));
  } else if (stage == kGeometryStage) {
    d3d_context_->GSSetConstantBuffers(0, 1, &(constants->buffer_));
  } else if (stage == kPixelStage) {
    d3d_context_->PSSetConstantBuffers(0, 1, &(constants->buffer_));
  } else {
    CHECK(false);
  }
}

inline void D3D11Renderer::ResetTexture(RenderPipelineStage stage, int index) {
  DCHECK(NULL != d3d_context_);
  switch (stage) {
    case kPixelStage:
      d3d_context_->PSSetShaderResources(index, 0, NULL);
      break;
    default:
      CHECK(false);
  }
}

void D3D11Renderer::UseTexture(RenderPipelineStage stage, int index,
                               Texture* texture) {
  D3D11Texture2D* tex = (D3D11Texture2D*)texture;
  if (tex) {
    tex->UseForStage(stage, index, this);
  } else {
    ResetTexture(stage, index);
  }
}

void D3D11Renderer::SetViewport(const Viewport& vp) {
  D3D11_VIEWPORT viewport;
  ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
  viewport.TopLeftX= vp.left;
  viewport.TopLeftY= vp.top;
  viewport.Width= vp.width;
  viewport.Height= vp.height;
  viewport.MinDepth= vp.min_depth;
  viewport.MaxDepth= vp.max_depth;
  d3d_context_->RSSetViewports(1, &viewport);
  viewport_ = vp;
}

void D3D11Renderer::SetShaderResource(RenderPipelineStage stage,
                                      uint32 first, uint32 num,
                                      ID3D11ShaderResourceView** view) {
  DCHECK(d3d_context_ != NULL);
  switch (stage) {
    case kVertexStage: d3d_context_->VSSetShaderResources(first, num, view);
    case kPixelStage: d3d_context_->PSSetShaderResources(first, num, view);
    default: CHECK(false);
  }
}

bool D3D11Renderer::Init(const Texture::Options& o) {
  DCHECK(o.width != 0 && o.height != 0);
  DCHECK(targets_[0].get() == NULL);
  DCHECK(depth_.get() == NULL);

  RenderTargetPtr target(D3D11RenderTarget::Create(o, this));
  DepthBufferPtr depth(D3D11DepthBuffer::Create(o, this));
  if (target.get() == NULL || depth.get()) {
    return false;
  }

  Reset();
  SetViewport(azer::Renderer::Viewport(0, 0, o.width, o.height));

  targets_[0] = target;
  depth_ = depth;
  return true;
}

/**
 * 在调用 ID3D11DeviceContext::GetRSState() 之前必须首先调用 CreateTasterizerState
 * 此函数会在初始化结束之后调用。
 * 如果初始化成功，那么之后的代码就可以放心大胆的调用 SetCullingMode 等函数了
 * 1. Stackoverflow: RSGetState in out parameter returns null pointer? [http://stackoverflow.com/questions/16874481/rsgetstate-in-out-parameter-returns-null-pointer]
 *
 */
void D3D11Renderer::InitRenderState() {
  ID3D11RasterizerState* obj = NULL;
  DCHECK(d3d_context_ != NULL); 
  D3D11_RASTERIZER_DESC desc;
  ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
  desc.FillMode = D3D11_FILL_SOLID;
  desc.CullMode = D3D11_CULL_BACK;
  desc.FrontCounterClockwise = FALSE;
  desc.DepthBias = 0;
  desc.SlopeScaledDepthBias = 0.0f;
  desc.DepthBiasClamp = 0.0f;
  desc.DepthClipEnable = TRUE;
  desc.ScissorEnable = FALSE;
  desc.MultisampleEnable = TRUE;
  desc.AntialiasedLineEnable = TRUE;
  HRESULT hr = d3d11_render_system_->GetDevice()->CreateRasterizerState(&desc, &obj);
  HRESULT_HANDLE_NORET(hr, ERROR, "CreateTasterizerState failed ");
  D3DObjPtr auto_ptr(obj);
  d3d_context_->RSSetState(obj);
}

bool D3D11SurfaceRenderer::InitForSurface(RenderTargetPtr target,
                                          DepthBufferPtr depth) {
  if (target.get() == NULL || depth.get() == NULL) {
    return false;
  }

  int32 width = surface_->GetBounds().width();
  int32 height = surface_->GetBounds().height();
  targets_.resize(1);
  targets_[0] = target;
  depth_ = depth;
  Reset();
  SetViewport(azer::Renderer::Viewport(0, 0, width, height));
  return true;
}
}  // namespace azer
