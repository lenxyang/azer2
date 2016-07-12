#include "azer/render_system/d3d11/gpu_buffer_view.h"

namespace azer {
namespace d3d11 {

// class D3DUAStructuredGpuBufferView
D3DShaderResView::D3DShaderResView(const Options& opt, GpuBuffer* buffer)
    : ShaderResView(opt, buffer),
      res_view_(NULL) {
}

D3DShaderResView::~D3DShaderResView() {
  SAFE_RELEASE(res_view_);
}

bool D3DShaderResView::Init(D3DRenderSystem* rs) {
  HRESULT hr;
  D3D11_SHADER_RESOURCE_VIEW_DESC desc = {
    DXGI_FORMAT_UNKNOWN,
    D3D11_UAV_DIMENSION_BUFFER;
    0,
    0,
  };

  ZeroMemory(&desc, sizeof(desc));
  desc.Format = DXGI_FORMAT_UNKNOWN;
  desc.ViewDimension = 
  desc.Buffer.NumElements = buffer->count();
  ID3D11Device* d3ddevice = rs->GetDevice();
  hr = d3ddevice->CreateUnorderedAccessView(buffer->object(), &desc, &unorder_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateShaderResourceView failed");
  return true;
}

// class D3DUAStructuredGpuBufferView
D3DUnorderAccessResView::D3DUnorderAccessResView(const Options& opt, 
                                                 GpuBuffer* buffer)
    : UnorderAccessResView(opt, buffer),
      unorder_view_(NULL) {
}

D3DUnorderAccessResView::~D3DUnorderAccessResView() {
  SAFE_RELEASE(unorder_view_);
}

bool D3DUnorderAccessResView::Init(D3DRenderSystem* rs) {
  HRESULT hr;
  D3DStructuredGpuBuffer* buffer = (D3DStructuredGpuBuffer*)(
  D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Format = DXGI_FORMAT_UNKNOWN;
  desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
  desc.Buffer.NumElements = buffer->count();
  ID3D11Device* d3ddevice = rs->GetDevice();
  hr = d3ddevice->CreateUnorderedAccessView(buffer->object(), &desc, &unorder_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateShaderResourceView failed");
  return true;
}
}  // namespace azer
}  // namespace d3d11
