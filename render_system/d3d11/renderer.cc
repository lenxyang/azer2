#include "azer/render_system/d3d11/renderer.h"

#include "base/strings/string16.h"

#include "base/logging.h"

#include "azer/render/surface.h"
#include "azer/render/render_system.h"
#include "azer/render_system/d3d11/blending.h"
#include "azer/render_system/d3d11/depth_buffer.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_buffer_view.h"
#include "azer/render_system/d3d11/gpu_constants_table.h"
#include "azer/render_system/d3d11/indices_buffer.h"
#include "azer/render_system/d3d11/sampler_state.h"
#include "azer/render_system/d3d11/structured_buffer.h"
#include "azer/render_system/d3d11/render_target.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/technique.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/texture_view.h"
#include "azer/render_system/d3d11/vertex_buffer.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {

namespace {
D3D_PRIMITIVE_TOPOLOGY TranslatePrimitiveTopology(
    PrimitiveTopology topology) {
  switch(topology) {
    case kPointList: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
    case kLineStrip: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
    case kLineList: return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
    case kTriangleStrip: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    case kTriangleList: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    case kControlPoint1: return D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;
    case kControlPoint2: return D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;
    case kControlPoint3: return D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
    case kControlPoint4: return D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
    case kControlPoint5: return D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST;
    case kControlPoint6: return D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST;
    case kControlPoint7: return D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST;
    case kControlPoint8: return D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST;
    case kControlPoint9: return D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST;
    case kControlPoint10: return D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST;
    case kControlPoint11: return D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST;
    case kControlPoint12: return D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST;
    case kControlPoint13: return D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST;
    case kControlPoint14: return D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST;
    case kControlPoint15: return D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST;
    case kControlPoint16: return D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST;
    case kControlPoint17: return D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST;
    case kControlPoint18: return D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST;
    case kControlPoint19: return D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST;
    case kControlPoint20: return D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST;
    case kControlPoint21: return D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST;
    case kControlPoint22: return D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST;
    case kControlPoint23: return D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST;
    case kControlPoint24: return D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST;
    case kControlPoint25: return D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
    case kControlPoint26: return D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST;
    case kControlPoint27: return D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST;
    case kControlPoint28: return D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST;
    case kControlPoint29: return D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST;
    case kControlPoint30: return D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST;
    case kControlPoint31: return D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST;
    case kControlPoint32: return D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST;
    default:
      DCHECK(false) << "Unsupport type: " << (int)topology;
      return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
  }
}
}

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
  ReportDeviceObjects(d3d11_render_system_);
  SAFE_RELEASE(d3d_context_);
}

void D3DRenderer::Use() {
  // DCHECK(!targets_.empty() && targets_[0].get() != NULL);
  DCHECK(targets_.empty() || depth_.get() != NULL);

  ID3D11RenderTargetView* target_view[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = {0};
  int count = 0;
  for (uint32_t i = 0; i < targets_.size(); ++i, ++count) {
    D3DRenderTarget* target = ((D3DRenderTarget*)targets_[i].get());
    target_view[i] = target->GetD3DRenderTargetView();
  }

  
  DCHECK_GE(count, 0);
  ID3D11DepthStencilView* depth_view = NULL;
  
  if (depth_.get()) {
    D3DDepthBuffer* depth = ((D3DDepthBuffer*)depth_.get());
    DCHECK(targets_.size() == 0u || depth->size() == targets_[0]->size());
    depth_view = depth->GetD3DDepthStencilView();
  }

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

void D3DRenderer::SetStreamOutTargets(GpuBuffer** buffer, int count, 
                                      int offset) {
  const int kMaxBufferCount = 64;
  DCHECK_LT(count, kMaxBufferCount);
  ID3D11Buffer* buffers[kMaxBufferCount] = { 0 };
  uint32_t offsets[kMaxBufferCount] = { 0 };
  for (int i = 0; i < count; ++i) {
    DCHECK(dynamic_cast<D3DVertexBuffer*>(*(buffer + i)));
    D3DVertexBuffer* vb = static_cast<D3DVertexBuffer*>(*(buffer + i));
    buffers[i] = vb->buffer();
  }

  d3d_context_->SOSetTargets(count, buffers, offsets);
}

void D3DRenderer::SetBlending(Blending* vblending, float* factor, uint32_t mask) {
  DCHECK(NULL != d3d_context_);
  D3DBlending* blending = (D3DBlending*)vblending;
  DCHECK(NULL != blending->blending_state_);
  d3d_context_->OMSetBlendState(blending->blending_state_, factor, mask);
}

void D3DRenderer::Clear(const azer::Vector4& color) {
  DCHECK(d3d_context_ != NULL);
  // DCHECK(targets_[0].get() != NULL);

  for (auto iter = targets_.begin(); iter != targets_.end(); ++iter) {
    DCHECK(dynamic_cast<D3DRenderTarget*>(iter->get()));
    D3DRenderTarget* target = (D3DRenderTarget*)iter->get();
    ID3D11RenderTargetView* target_view = target->GetD3DRenderTargetView();
    DCHECK(target_view != NULL);
    d3d_context_->ClearRenderTargetView(target_view, (float*)&color);
  }
}

void D3DRenderer::ClearDepthAndStencil(bool clear_depth, bool clear_stencil,
                                       float depth_val, int stencil_val) {
  DCHECK(NULL != depth_.get());
  D3DDepthBuffer* depth = ((D3DDepthBuffer*)depth_.get());
  depth->Clear(this, clear_depth, clear_stencil, depth_val, stencil_val);
}

void D3DRenderer::SetShader(int stage, Shader* shader) {
  DCHECK_EQ(shader->stage(), stage);
  switch (shader->stage()) {
    case kVertexStage:
      d3d_context_->VSSetShader(((D3DVertexShader*)shader)->handle(), 0, 0);
      break;
    case kGeometryStage:
      d3d_context_->GSSetShader(((D3DGeometryShader*)shader)->handle(), 0, 0);
      break;
    case kDomainStage:
      d3d_context_->DSSetShader(((D3DDomainShader*)shader)->handle(), 0, 0);
      break;
    case kHullStage:
      d3d_context_->HSSetShader(((D3DHullShader*)shader)->handle(), 0, 0);
      break;
    case kComputeStage:
      d3d_context_->CSSetShader(((D3DComputeShader*)shader)->handle(), 0, 0);
      break;
    case kPixelStage:
      d3d_context_->PSSetShader(((D3DPixelShader*)shader)->handle(), 0, 0);
      break;
    default:
      CHECK(false) << "No such GpuProgram Type: " << (int)stage;
  }
}

void D3DRenderer::ResetShader(RenderPipelineStage stage) {
  switch (stage) {
    case kVertexStage:
      d3d_context_->VSSetShader(NULL, 0, 0);
      break;
    case kGeometryStage:
      d3d_context_->GSSetShader(NULL, 0, 0);
      break;
    case kDomainStage:
      d3d_context_->DSSetShader(NULL, 0, 0);
      break;
    case kHullStage:
      d3d_context_->HSSetShader(NULL, 0, 0);
      break;
    case kComputeStage:
      d3d_context_->CSSetShader(NULL, 0, 0);
      break;
    case kPixelStage:
      d3d_context_->PSSetShader(NULL, 0, 0);
      break;
    default:
      CHECK(false) << "No such GpuProgram Type: " << (int)stage;
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

void D3DRenderer::Draw(int vertices_num, int start_vertex) {
  d3d_context_->Draw(vertices_num, start_vertex);
}

void D3DRenderer::DrawIndex(int indices_num, int start_indices, 
                            int vertex_base) {
  d3d_context_->DrawIndexed(indices_num, start_indices, vertex_base);
}

void D3DRenderer::DrawInstanced(int instance_num, int vertices_num, 
                                int first_vertex, int instance_start_index) {
  d3d_context_->DrawInstanced(vertices_num, instance_num,
                              first_vertex, instance_start_index);
}

void D3DRenderer::DrawIndexInstanced(int instance_num, int indices_num, 
                                     int first_indices, int index_base,
                                     int instance_start_index) {
  d3d_context_->DrawIndexedInstanced(indices_num, instance_num,
                                     first_indices, index_base,
                                     instance_start_index);
}

void D3DRenderer::DispatchComputeTask(const GpuTaskParams& params) {
  ID3D11DeviceContext* context = GetContext();
  context->Dispatch(params.thread_group.x, params.thread_group.y,
                    params.thread_group.z);
}

void D3DRenderer::BindConstantsTable(RenderPipelineStage stage, int index,
                                     GpuConstantsTable* table) {
  D3DGpuConstantsTable* constants = (D3DGpuConstantsTable*)table;
  switch (stage) {
    case kVertexStage:
      d3d_context_->VSSetConstantBuffers(index, 1, &(constants->buffer_));
      break;
    case kGeometryStage:
      d3d_context_->GSSetConstantBuffers(index, 1, &(constants->buffer_));
      break;
    case kHullStage:
      d3d_context_->HSSetConstantBuffers(index, 1, &(constants->buffer_));
      break;
    case kDomainStage:
      d3d_context_->DSSetConstantBuffers(index, 1, &(constants->buffer_));
      break;
    case kPixelStage:
      d3d_context_->PSSetConstantBuffers(index, 1, &(constants->buffer_));
      break;
    default:
      CHECK(false);
      break;
  }
}

void D3DRenderer::ResetStageResource(RenderPipelineStage stage) {
  const int count = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
  ID3D11ShaderResourceView* views[count] = {0};
  DCHECK(NULL != d3d_context_);
  switch (stage) {
    case kVertexStage:
      d3d_context_->VSSetShaderResources(0, count, views);
      break;
    case kHullStage:
      d3d_context_->HSSetShaderResources(0, count, views);
      break;
    case kDomainStage:
      d3d_context_->DSSetShaderResources(0, count, views);
      break;
    case kPixelStage:
      d3d_context_->PSSetShaderResources(0, count, views);
      break;
    default:
      CHECK(false);
  }
}

void D3DRenderer::SetShaderSamplerState(RenderPipelineStage stage, int index, 
                                        int count, SamplerStatePtr* sampler) {
  const int kMaxShaderTexCount = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
  DCHECK_LT(count, kMaxShaderTexCount);
  ID3D11SamplerState* sampler_state[kMaxShaderTexCount] = {0};
  for (int i = 0; i < count; ++i) {
    D3DSamplerState* p = (D3DSamplerState*)sampler[i].get();
    sampler_state[i] = p->GetD3DSamplerState();
    DCHECK(sampler_state[i]);
  }

  switch (stage) {
    case kVertexStage: 
      d3d_context_->VSSetSamplers(index, count, sampler_state);
      break;
    case kHullStage: 
      d3d_context_->HSSetSamplers(index, count, sampler_state);
      break;
    case kDomainStage: 
      d3d_context_->DSSetSamplers(index, count, sampler_state);
      break;
    case kGeometryStage: 
      d3d_context_->GSSetSamplers(index, count, sampler_state);
      break;
    case kPixelStage: 
      d3d_context_->PSSetSamplers(index, count, sampler_state);
      break;
    default: CHECK(false);
  }
}

void D3DRenderer::SetShaderResource(RenderPipelineStage stage, int index, 
                                    int count, ResourceViewPtr* res) {
  const int kMaxShaderTexCount = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
  DCHECK_LT(count, kMaxShaderTexCount);
  ID3D11ShaderResourceView* views[kMaxShaderTexCount] = {0};
  ResourceViewPtr* cur = (ResourceViewPtr*)res;
  for (int i = 0; i < count; ++i, ++cur) {
    D3DShaderResView* tex = (D3DShaderResView*)(cur->get());
    views[i] = tex ? tex->GetResourceView() : NULL;
  }
  
  switch (stage) {
    case kVertexStage: 
      d3d_context_->VSSetShaderResources(index, count, views);
      break;
    case kHullStage: 
      d3d_context_->HSSetShaderResources(index, count, views);
      break;
    case kDomainStage: 
      d3d_context_->DSSetShaderResources(index, count, views);
      break;
    case kGeometryStage: 
      d3d_context_->GSSetShaderResources(index, count, views);
      break;
    case kPixelStage: 
      d3d_context_->PSSetShaderResources(index, count, views);
      break;
    default: CHECK(false);
  }
}

void D3DRenderer::SetShaderUAResource(RenderPipelineStage stage, int index, 
                                      int count, UnorderAccessResView* res) {
  const int kMaxShaderTexCount = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
  DCHECK_LT(count, kMaxShaderTexCount);
  ID3D11UnorderedAccessView* views[kMaxShaderTexCount] = {0};
  TextureViewPtr* cur = (TextureViewPtr*)res;
  for (int i = 0; i < count; ++i, ++cur) {
    D3DUnorderAccessResView* tex = (D3DUnorderAccessResView*)(cur->get());
    views[i] = tex ? tex->GetResourceView() : NULL;
  }
  
  switch (stage) {
    case kComputeStage:
      d3d_context_->CSSetUnorderedAccessViews(index, count, views, (UINT*)&views);
      break;
    case kVertexStage: 
    case kHullStage: 
    case kDomainStage: 
    case kGeometryStage: 
    case kPixelStage:
    default: CHECK(false) << "UnorderedAccess just support ComputeStage";
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

bool D3DRenderer::Init(std::vector<RenderTargetPtr>* rt, DepthBuffer* depth) {
  DCHECK_EQ(targets_.size(), 0u);
  DCHECK_GE(rt->size(), 0u);
  for (auto iter = rt->begin(); iter != rt->end(); ++iter) {
    targets_.push_back(*iter);
  }
  depth_ = depth;
  Reset();
  gfx::Size size;
  if (depth) {
    size = depth->size();
  } else {
    CHECK_GT(rt->size(), 0u);
    size = rt->at(0)->size();
  }
  SetViewport(Viewport(0, 0, size.width(), size.height()));
  return true;
}
}  // namespace d3d11
}  // namespace azer
