#include "azer/render_system/d3d11/indices_buffer.h"

#include <d3dx11.h>

#include "azer/render/indices_buffer.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"

namespace azer {
namespace d3d11 {
D3DIndicesBuffer::D3DIndicesBuffer(const IndicesBuffer::Options& opt,
                                   D3DRenderSystem* rs)
    : IndicesBuffer(opt)
    , locked_(false)
    , buffer_(NULL)
    , render_system_(rs) {
}

D3DIndicesBuffer::~D3DIndicesBuffer() {
  SAFE_RELEASE(buffer_);
}

bool D3DIndicesBuffer::Init(const IndicesData* data) {
  DCHECK(NULL == buffer_);
  DCHECK(data != NULL);
  DCHECK(indices_count_ == -1 && type_ == kIndexUndefined);

  ID3D11Device* d3d_device = render_system_->GetDevice();

  D3D11_BUFFER_DESC indices_buffer_desc;
  ZeroMemory(&indices_buffer_desc, sizeof(indices_buffer_desc));
  indices_buffer_desc.Usage = TranslateUsage(options_.usage);
  indices_buffer_desc.ByteWidth = data->size();
  indices_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indices_buffer_desc.CPUAccessFlags = TranslateCPUAccess(options_.cpu_access);;
  indices_buffer_desc.MiscFlags = 0;

  D3D11_SUBRESOURCE_DATA d3d_vdata;
  ZeroMemory(&d3d_vdata, sizeof(d3d_vdata));
  d3d_vdata.pSysMem = data->pointer();
  HRESULT hr;
  hr = d3d_device->CreateBuffer(&indices_buffer_desc, &d3d_vdata, &buffer_);
  HRESULT_HANDLE(hr, ERROR, "D3D11: CreateIndexBuffer failed ");

  indices_count_ = data->num();
  type_ = data->type();

  return true;
}

HardwareBufferDataPtr D3DIndicesBuffer::map(MapType flags) {
  DCHECK(options_.usage & GraphicBuffer::kDynamic
         || options_.usage & GraphicBuffer::kStaging);
  DCHECK(options_.cpu_access & kCPUWrite || options_.cpu_access & kCPURead);
  HRESULT hr;
  DCHECK(!locked_) << "Indices Buffer("") has been locked";
  ID3D11Device* d3d_device = render_system_->GetDevice();
  ID3D11DeviceContext* d3d_context = NULL;
  d3d_device->GetImmediateContext(&d3d_context);
  ScopedRefCOM autocom(d3d_context);
  D3D11_MAPPED_SUBRESOURCE mapped;
  D3D11_MAP d3d11_map = TranslateMapType(flags);
  hr = d3d_context->Map(buffer_, 0, d3d11_map, 0, &mapped);
  if (FAILED(hr)) {
    LOG(ERROR) << "D3D11 IndicesBuffer: Map failed, desc: " << HRMessage(hr);
    return NULL;
  }

  HardwareBufferDataPtr data(new HardwareBufferData);
  SetLockDataPtr(mapped.pData, data.get());
  SetLockDataRowSize(mapped.RowPitch, data.get());
  SetLockDataColumnNum(mapped.DepthPitch, data.get());
  locked_ = true;
  return data;
}

void D3DIndicesBuffer::unmap() {
  DCHECK(locked_) << "Indices Buffer has not been locked";
  ID3D11Device* d3d_device = render_system_->GetDevice();
  ID3D11DeviceContext* d3d_context = NULL;
  d3d_device->GetImmediateContext(&d3d_context);
  ScopedRefCOM autocom(d3d_context);
  d3d_context->Unmap(buffer_, 0);
  locked_ = false;
}
}  // namespace d3d11
}  // namespace azer
