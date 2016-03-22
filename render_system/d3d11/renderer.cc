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
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/technique.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/vertex_buffer.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {

const std::string& D3DRenderer::name_ = "Direct3D11Renderer";

const std::string& D3DRenderer::name() const {
  return name_;
}

D3DRenderer::D3DRenderer(ID3D11DeviceContext* context, D3DRenderSystem* rs)
    : Renderer(rs)
    , d3d11_render_system_(rs)
    , d3d_context_(context) {
}

D3DRenderer::~D3DRenderer() {
  SAFE_RELEASE(d3d_context_);
}

void D3DRenderer::Use() {
  DCHECK(!targets_.empty() && targets_[0].get() != NULL);
  DCHECK(depth_.get() != NULL);

  ID3D11RenderTargetView* target_view[32] = {0};
  int32 count = 0;
  for (uint32 i = 0; i < targets_.size(); ++i, ++count) {
    D3DRenderTarget* target = ((D3DRenderTarget*)targets_[i].get());
    target_view[i] = target->GetD3DRenderTargetView();
  }

  D3DDepthBuffer* depth = ((D3DDepthBuffer*)depth_.get());
  ID3D11DepthStencilView* depth_view = depth->GetD3DDepthStencilView();
  d3d_context_->OMSetRenderTargets(count, target_view, depth_view);

  // reset all state to current
  SetViewport(viewport_);
}

void D3DRenderer::Reset() {
  DCHECK(d3d_context_ != NULL);
  d3d_context_->ClearState();
  ResetRasterizerState();
  ResetDepthStencilState();
}

void D3DRenderer::ResetBlending() {
  d3d_context_->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void D3DRenderer::BindVertexBuffer(VertexBuffer* vvb) {
  if (vvb) {
    DCHECK(typeid(*vvb) == typeid(D3DVertexBuffer));
    D3DVertexBuffer* vb = static_cast<D3DVertexBuffer*>(vvb);
    D3DVertexLayout* layout = static_cast<D3DVertexLayout*>(vb->vertex_layout());
    UINT stride = vb->element_size();
    UINT offset = 0;
    ID3D11Buffer* buf = vb->buffer();
    d3d_context_->IASetInputLayout(layout->input_layout());
    d3d_context_->IASetVertexBuffers(0, 1, &buf, &stride, &offset);
  } else {
    d3d_context_->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
  }
}

void D3DRenderer::BindVertexBufferGroup(VertexBufferGroup* vbg) {
  DCHECK(typeid(*vbg) == typeid(D3DVertexBufferGroup));
  D3DVertexBufferGroup* vg = static_cast<D3DVertexBufferGroup*>(vbg);
  D3DVertexLayout* layout = static_cast<D3DVertexLayout*>(vg->vertex_layout());
  d3d_context_->IASetInputLayout(layout->input_layout());
  d3d_context_->IASetVertexBuffers(0, vg->vertex_buffer_count(),
                                   vg->buffer(), vg->strides(), vg->offsets());
}

void D3DRenderer::BindIndicesBuffer(IndicesBuffer* vib) {
  if (vib) {
    DCHECK(typeid(*vib) == typeid(D3DIndicesBuffer));
    D3DIndicesBuffer* ib = static_cast<D3DIndicesBuffer*>(vib);
    d3d_context_->IASetIndexBuffer(ib->buffer(), TranslateIndexType(ib->type()), 0);
  } else {
    // d3d_context_->IASetIndexBuffer(NULL, NULL, 0);
  }
}

void D3DRenderer::SetStreamOutTargets(HardwareBuffer** buffer, int count, 
                                      int offset) {
  const int kMaxBufferCount = 64;
  DCHECK_LT(count, kMaxBufferCount);
  ID3D11Buffer* buffers[kMaxBufferCount] = { 0 };
  uint32 offsets[kMaxBufferCount] = { 0 };
  for (int i = 0; i < count; ++i) {
    DCHECK(dynamic_cast<D3DVertexBuffer*>(*(buffer + i)));
    D3DVertexBuffer* vb = static_cast<D3DVertexBuffer*>(*(buffer + i));
    buffers[i] = vb->buffer();
  }

  d3d_context_->SOSetTargets(count, buffers, offsets);
}

void D3DRenderer::SetBlending(Blending* vblending, float* factor, uint32 mask) {
  DCHECK(NULL != d3d_context_);
  D3DBlending* blending = (D3DBlending*)vblending;
  DCHECK(NULL != blending->blending_state_);
  d3d_context_->OMSetBlendState(blending->blending_state_, factor, mask);
}

void D3DRenderer::Clear(const azer::Vector4& color) {
  DCHECK(d3d_context_ != NULL);
  DCHECK(targets_[0].get() != NULL);

  for (auto iter = targets_.begin(); iter != targets_.end(); ++iter) {
    DCHECK(dynamic_cast<D3DRenderTarget*>(iter->get()));
    D3DRenderTarget* target = (D3DRenderTarget*)iter->get();
    ID3D11RenderTargetView* target_view = target->GetD3DRenderTargetView();
    DCHECK(target_view != NULL);
    D3DXCOLOR dxcolor(color.x, color.y, color.z, color.w);
    d3d_context_->ClearRenderTargetView(target_view, dxcolor);
  }
}

void D3DRenderer::ClearDepthAndStencil(bool clear_depth, bool clear_stencil,
                                       float depth_val, int stencil_val) {
  DCHECK(NULL != depth_.get());
  D3DDepthBuffer* depth = ((D3DDepthBuffer*)depth_.get());
  depth->Clear(this, clear_depth, clear_stencil, depth_val, stencil_val);
}

void D3DRenderer::ResetShader(RenderPipelineStage stage) {
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

void D3DRenderer::SetPrimitiveTopology(PrimitiveTopology primitive) {
  d3d_context_->IASetPrimitiveTopology(TranslatePrimitiveTopology(primitive));
}

void D3DRenderer::SetVertexLayout(VertexLayout* layout) {
  DCHECK(typeid(*layout) == typeid(D3DVertexLayout));
  D3DVertexLayout* d3dlayout = static_cast<D3DVertexLayout*>(layout);
  d3d_context_->IASetInputLayout(d3dlayout->input_layout());
}

void D3DRenderer::DrawAuto() {
  d3d_context_->DrawAuto();
}

void D3DRenderer::Draw(int vertices_num, int32 start_vertex) {
  d3d_context_->Draw(vertices_num, start_vertex);
}

void D3DRenderer::DrawIndex(int indices_num, int32 start_indices, 
                            int32 vertex_base) {
  d3d_context_->DrawIndexed(indices_num, start_indices, vertex_base);
}

void D3DRenderer::DrawInstanced(int32 instance_num, int32 vertices_num, 
                                int32 first_vertex, int32 instance_start_index) {
  d3d_context_->DrawInstanced(vertices_num, instance_num,
                              first_vertex, instance_start_index);
}

void D3DRenderer::DrawIndexInstanced(int32 instance_num, int32 indices_num, 
                                     int32 first_indices, int32 index_base,
                                     int32 instance_start_index) {
  d3d_context_->DrawIndexedInstanced(indices_num, instance_num,
                                     first_indices, index_base,
                                     instance_start_index);
}

void D3DRenderer::BindConstantsTable(RenderPipelineStage stage,
                                     GpuConstantsTable* table) {
  D3DGpuConstantsTable* constants = (D3DGpuConstantsTable*)table;
  if (stage == kVertexStage) {
    d3d_context_->VSSetConstantBuffers(0, 1, &(constants->buffer_));
  } else if (stage == kGeometryStage) {
    d3d_context_->GSSetConstantBuffers(0, 1, &(constants->buffer_));
  } else if (stage == kHullStage) {
    d3d_context_->HSSetConstantBuffers(0, 1, &(constants->buffer_));
  } else if (stage == kDomainStage) {
    d3d_context_->DSSetConstantBuffers(0, 1, &(constants->buffer_));
  } else if (stage == kPixelStage) {
    d3d_context_->PSSetConstantBuffers(0, 1, &(constants->buffer_));
  } else {
    CHECK(false);
  }
}

inline void D3DRenderer::ResetTexture(RenderPipelineStage stage, int index) {
  DCHECK(NULL != d3d_context_);
  switch (stage) {
    case kVertexStage:
      d3d_context_->VSSetShaderResources(index, 0, NULL);
      break;
    case kHullStage:
      d3d_context_->HSSetShaderResources(index, 0, NULL);
      break;
    case kDomainStage:
      d3d_context_->DSSetShaderResources(index, 0, NULL);
      break;
    case kPixelStage:
      d3d_context_->PSSetShaderResources(index, 0, NULL);
      break;
    default:
      CHECK(false);
  }
}

void D3DRenderer::BindTexture(RenderPipelineStage stage, int index,
                             Texture* texture) {
  D3DTexture2D* tex = (D3DTexture2D*)texture;
  if (tex) {
    tex->UseForStage(stage, index, this);
  } else {
    ResetTexture(stage, index);
  }
}

const Viewport& D3DRenderer::GetViewport() const {
  return viewport_;
}

void D3DRenderer::SetViewport(const Viewport& vp) {
  D3D11_VIEWPORT viewport;
  ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
  viewport.TopLeftX = vp.bounds.x();
  viewport.TopLeftY = vp.bounds.y();
  viewport.Width = vp.bounds.width();
  viewport.Height = vp.bounds.height();
  viewport.MinDepth = vp.min_depth;
  viewport.MaxDepth = vp.max_depth;
  d3d_context_->RSSetViewports(1, &viewport);
  viewport_ = vp;
}

void D3DRenderer::SetShaderResource(RenderPipelineStage stage,
                                    uint32 first, uint32 num,
                                    ID3D11ShaderResourceView** view) {
  DCHECK(d3d_context_ != NULL);
  switch (stage) {
    case kVertexStage: d3d_context_->VSSetShaderResources(first, num, view);
    case kPixelStage: d3d_context_->PSSetShaderResources(first, num, view);
    default: CHECK(false);
  }
}

bool D3DRenderer::Init(RenderTarget* rt, DepthBuffer* depth) {
  DCHECK(rt && depth);
  DCHECK_EQ(targets_.size(), 0u);
  targets_.push_back(rt);
  depth_ = depth;
  Reset();
  const Texture::Options& o = rt->GetTexture()->options();
  SetViewport(Viewport(0, 0, o.size.width(), o.size.height()));
  return true;
}

bool D3DRenderer::Init(std::vector<RenderTargetPtr>* rt, DepthBuffer* depth) {
  DCHECK_EQ(targets_.size(), 0u);
  DCHECK_GT(rt->size(), 0u);
  targets_.swap(*rt);
  depth_ = depth;
  Reset();
  const Texture::Options& o = targets_[0]->GetTexture()->options();
  SetViewport(Viewport(0, 0, o.size.width(), o.size.height()));
  return true;
}

bool D3DRenderer::Init(const Texture::Options& o) {
  DCHECK(!o.size.IsEmpty());
  DCHECK_EQ(targets_.size(), 0u);
  DCHECK(depth_.get() == NULL);

  RenderTargetPtr target(D3DRenderTarget::Create(o, d3d11_render_system_));
  DepthBufferPtr depth(D3DDepthBuffer::Create(o, d3d11_render_system_));
  if (target.get() == NULL || depth.get() == NULL) {
    return false;
  }

  targets_.push_back(target);
  depth_ = depth;
  Reset();
  SetViewport(Viewport(0, 0, o.size.width(), o.size.height()));
  return true;
}

bool D3DSurfaceRenderer::InitForSurface(RenderTarget* target, DepthBuffer* depth) {
  DCHECK(target != NULL || depth != NULL);

  int32 width = surface_->GetBounds().width();
  int32 height = surface_->GetBounds().height();
  targets_.resize(1);
  targets_[0] = target;
  depth_ = depth;
  Reset();
  SetViewport(Viewport(0, 0, width, height));
  return true;
}
}  // namespace d3d11
}  // namespace azer
