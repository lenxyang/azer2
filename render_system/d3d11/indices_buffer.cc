#include "azer/render_system/d3d11/indices_buffer.h"

#include "azer/render/indices_buffer.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_buffer_map_helper.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"

namespace azer {
namespace d3d11 {
D3DIndicesBuffer::D3DIndicesBuffer(const GpuBufferOptions& opt)
    : IndicesBuffer(opt)
    , locked_(false)
    , buffer_(NULL) {
}

D3DIndicesBuffer::~D3DIndicesBuffer() {
  SAFE_RELEASE(buffer_);
}

bool D3DIndicesBuffer::Init(const IndicesData* data, D3DRenderSystem* rs) {
  DCHECK(NULL == buffer_);
  DCHECK(data != NULL);
  DCHECK(indices_count_ == -1 && type_ == kIndexUndefined);

  ID3D11Device* d3d_device = rs->GetDevice();

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

  indices_count_ = data->count();
  type_ = data->type();

  return true;
}

GpuBufferDataPtr D3DIndicesBuffer::map(MapType flags) {
  map_helper_.reset(new GpuBufferMapHelper(options(), buffer_));
  return map_helper_->map(flags);
}

void D3DIndicesBuffer::unmap() {
  CHECK(map_helper_.get());
  map_helper_->unmap();
  map_helper_->reset();
}
}  // namespace d3d11
}  // namespace azer
