#include "azer/render_system/d3d11/vertex_buffer.h"

#include <sstream>
#include <windows.h>
#include "base/logging.h"
#include "base/strings/string_util.h"
#include "azer/base/string.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/enum_transform.h"
#include "azer/render_system/d3d11/gpu_res_lock_helper.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/vertex_layout.h"

namespace azer {
namespace d3d11 {
// class D3DVertexBuffer
D3DVertexBuffer::D3DVertexBuffer(const GpuResOptions &opt)
    : VertexBuffer(opt)
    , locked_(false)
    , buffer_(NULL) {
}

D3DVertexBuffer::~D3DVertexBuffer() {
  SAFE_RELEASE(buffer_);
}
bool D3DVertexBuffer::Init(SlotVertexData* dataptr, D3DRenderSystem* rs) {
  DCHECK(element_size_ == -1 && buffer_size_ == -1 && vertex_count_ == -1);
  ID3D11Device* d3d_device = rs->GetDevice();

  // attention
  // If the bind flag is D3D11_BIND_CONSTANT_BUFFER, you must set the
  // ByteWidth value in multiples of 16, and less than or equal
  // to D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT.

  D3D11_BUFFER_DESC vb_desc;
  GenBufferDesc(resource_options(), &vb_desc);
  vb_desc.ByteWidth = dataptr->buffer_size();

  D3D11_SUBRESOURCE_DATA d3d_vdata;
  ZeroMemory(&d3d_vdata, sizeof(d3d_vdata));
  d3d_vdata.pSysMem = dataptr->pointer();
  HRESULT hr;
  hr = d3d_device->CreateBuffer(&vb_desc, &d3d_vdata, &buffer_);
  HRESULT_HANDLE(hr, ERROR, "D3D11: CreateVertexBuffer failed ");

  element_size_ = dataptr->stride();
  buffer_size_ = dataptr->buffer_size();
  vertex_count_ = dataptr->vertex_count();

  layout_ = new D3DVertexLayout(dataptr->vertex_desc());
  CHECK(layout_->Init(rs));

  return true;
}

void D3DVertexBuffer::SetName(const std::string& name) {
  DCHECK(buffer_);
  buffer_->SetPrivateData(WKPDID_D3DDebugObjectName,
                          (UINT)name.length(), name.c_str());
}

GpuResLockDataPtr D3DVertexBuffer::map(MapType flags) {
  map_helper_.reset(new GpuResLockHelper(resource_options(), buffer_));
  return map_helper_->map(flags);
}

void D3DVertexBuffer::unmap() {
  CHECK(map_helper_.get());
  map_helper_->unmap();
  map_helper_.reset();
}

bool D3DVertexBuffer::CopyTo(GpuResource* buffer)  {
  CHECK(false);
  return false;
}

// class D3DVertexBufferGroup
D3DVertexBufferGroup::D3DVertexBufferGroup(VertexDesc* desc, D3DRenderSystem* rs)
    : VertexBufferGroup(desc) {
  memset(vbs_, 0, sizeof(vbs_));
  memset(stride_, 0, sizeof(stride_));
  memset(offset_, 0, sizeof(offset_));
  layout_ = new D3DVertexLayout(desc);
  CHECK(layout_->Init(rs));
}

void D3DVertexBufferGroup::OnVertexBufferChanged() {
  GenVertexArray();
}

int32_t D3DVertexBufferGroup::GenVertexArray() {
  for (int i = 0; i < vertex_buffer_count(); ++i) {
    D3DVertexBuffer* vb = (D3DVertexBuffer*)(vertex_buffer_at(i));
    DCHECK(vb->Initialized()) << "VertexBuffer not initialized.";
    vbs_[i] = vb->buffer_;
    stride_[i] = vb->element_size();
    offset_[i] = 0;
  }
  return vertex_buffer_count();
}
}  // namespace d3d11
}  // namespace azer
