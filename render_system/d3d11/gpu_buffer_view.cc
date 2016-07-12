#include "azer/render_system/d3d11/gpu_buffer_view.h"

#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/d3d11.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/structured_buffer.h"

namespace azer {
namespace d3d11 {

// class D3DUAStructuredGpuBufferView
D3DStructuredBufferResView::D3DStructuredBufferResView(GpuBuffer* buffer)
    : ShaderResView(ResViewType::kStructuredBuffer, buffer),
      res_view_(NULL) {
}

D3DStructuredBufferResView::~D3DStructuredBufferResView() {
  SAFE_RELEASE(res_view_);
}

bool D3DStructuredBufferResView::Init(D3DRenderSystem* rs) {
  HRESULT hr;
  D3D11_SHADER_RESOURCE_VIEW_DESC desc = {
    DXGI_FORMAT_UNKNOWN,
    D3D11_SRV_DIMENSION_BUFFER,
    0,
    0,
  };

  D3DStructuredGpuBuffer* buf = (D3DStructuredGpuBuffer*)buffer();
  ZeroMemory(&desc, sizeof(desc));
  // desc.Format = DXGI_FORMAT_UNKNOWN;
  //  desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
  desc.Buffer.NumElements = buf->count();
  ID3D11Device* d3ddevice = rs->GetDevice();
  hr = d3ddevice->CreateShaderResourceView(buf->object(), &desc, &res_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateShaderResourceView failed");
  return true;
}

// class D3DUAStructuredGpuBufferView
D3DUAStructuredBufferResView::D3DUAStructuredBufferResView(GpuBuffer* buffer)
    : UnorderAccessResView(ResViewType::kUAStructuredBuffer, buffer),
      unorder_view_(NULL) {
}

D3DUAStructuredBufferResView::~D3DUAStructuredBufferResView() {
  SAFE_RELEASE(unorder_view_);
}

bool D3DUAStructuredBufferResView::Init(D3DRenderSystem* rs) {
  HRESULT hr;
  D3DStructuredGpuBuffer* buf = (D3DStructuredGpuBuffer*)(this->buffer());
  D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Format = DXGI_FORMAT_UNKNOWN;
  desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
  desc.Buffer.NumElements = buf->count();
  ID3D11Device* d3ddevice = rs->GetDevice();
  hr = d3ddevice->CreateUnorderedAccessView(buf->object(), &desc, &unorder_view_);
  HRESULT_HANDLE(hr, ERROR, "CreateShaderResourceView failed");
  return true;
}
}  // namespace azer
}  // namespace d3d11
