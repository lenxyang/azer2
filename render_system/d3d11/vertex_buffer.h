#pragma once


#include "azer/render/common.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
class D3DBlob;
class GpuResLockHelper;

class D3DVertexBuffer : public VertexBuffer {
 public:
  D3DVertexBuffer(const GpuResOptions &opt);
  virtual ~D3DVertexBuffer();
  bool Init(SlotVertexData* dataptr, D3DRenderSystem* rs);

  void SetName(const std::string& name) override;
  GpuResLockDataPtr map(MapType flags) override;
  void unmap() override;
  bool CopyTo(GpuResource* res) override;

  bool Initialized() const { return NULL != buffer_;}
  ID3D11Buffer* buffer() { return buffer_;}
  NativeGpuResourceHandle native_handle() override {
    return (NativeGpuResourceHandle)buffer();
  }
 private:
  bool locked_;
  ID3D11Buffer* buffer_;
  std::unique_ptr<GpuResLockHelper> map_helper_;
  friend class D3DVertexBufferGroup;
  DISALLOW_COPY_AND_ASSIGN(D3DVertexBuffer);
};

class D3DVertexBufferGroup : public VertexBufferGroup {
 public:
  explicit D3DVertexBufferGroup(VertexDesc* desc, D3DRenderSystem* rs);
  void OnVertexBufferChanged() override;

  ID3D11Buffer** buffer() { return vbs_;}
  uint32_t* strides() { return stride_;}
  uint32_t* offsets() { return offset_;}
 private:
  int32_t GenVertexArray();
  static const int32_t kMaxVertexBuffer = 16;
  ID3D11Buffer* vbs_[kMaxVertexBuffer];
  uint32_t stride_[kMaxVertexBuffer];
  uint32_t offset_[kMaxVertexBuffer];
  DISALLOW_COPY_AND_ASSIGN(D3DVertexBufferGroup);
};
}  // namespace d3d11
}  // namespace azer
