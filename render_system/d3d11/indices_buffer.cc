#include "azer/render_system/d3d11/indices_buffer.h"

#include "azer/render/indices_buffer.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_res_lock_helper.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"

namespace azer {
namespace d3d11 {
D3DIndicesBuffer::D3DIndicesBuffer(const GpuResOptions& opt)
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

  D3D11_BUFFER_DESC ibdesc;
  GenBufferDesc(resource_options(), &ibdesc);
  CHECK(ibdesc.BindFlags & D3D11_BIND_INDEX_BUFFER);
  ibdesc.ByteWidth = data->size();

  D3D11_SUBRESOURCE_DATA d3d_vdata;
  ZeroMemory(&d3d_vdata, sizeof(d3d_vdata));
  d3d_vdata.pSysMem = data->pointer();
  HRESULT hr;
  hr = d3d_device->CreateBuffer(&ibdesc, &d3d_vdata, &buffer_);
  HRESULT_HANDLE(hr, ERROR, "D3D11: CreateIndexBuffer failed ");

  indices_count_ = data->count();
  type_ = data->type();

  return true;
}

void D3DIndicesBuffer::SetName(const std::string& name) {
  DCHECK(buffer_);
  buffer_->SetPrivateData(WKPDID_D3DDebugObjectName,
                          (UINT)name.length(), name.c_str());
}


GpuResLockDataPtr D3DIndicesBuffer::map(MapType flags) {
  map_helper_.reset(new GpuResLockHelper(resource_options(), buffer_));
  return map_helper_->map(flags);
}

void D3DIndicesBuffer::unmap() {
  CHECK(map_helper_.get());
  map_helper_->unmap();
  map_helper_.reset();
}

bool D3DIndicesBuffer::CopyTo(GpuResource* res) {
  CHECK(false);
  return false;
}
}  // namespace d3d11
}  // namespace azer
