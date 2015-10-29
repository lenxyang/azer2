#pragma once

#include "base/basictypes.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
class D3DVertexBuffer : public VertexBuffer {
 public:
  virtual ~D3DVertexBuffer() {
    SAFE_RELEASE(buffer_);
  }
  
  bool Init(const SlotVertexData* dataptr);

  HardwareBufferDataPtr map(MapType flags) override;
  void unmap() override;

  bool Initialized() const { return NULL != buffer_;}
  ID3D11Buffer* buffer() { return buffer_;}
 private:
  D3DVertexBuffer(const Options &opt, D3DRenderSystem* rs)
      : VertexBuffer(opt)
      , locked_(false)
      , buffer_(NULL)
      , render_system_(rs) {
  }

  bool locked_;
  ID3D11Buffer* buffer_;
  D3DRenderSystem* render_system_;
  friend class D3DRenderSystem;
  friend class D3DRenderer;
  friend class D3DVertexBufferGroup;
  DISALLOW_COPY_AND_ASSIGN(D3DVertexBuffer);
};

class D3DVertexBufferGroup : public VertexBufferGroup {
 public:
  D3DVertexBufferGroup(VertexDescPtr desc);
  void OnVertexBufferChanged() override;

  ID3D11Buffer** buffer() { return vbs_;}
  uint32* strides() { return stride_;}
  uint32* offsets() { return offset_;}
 private:
  int32 GenVertexArray();
  static const int32 kMaxVertexBuffer = 16;
  ID3D11Buffer* vbs_[kMaxVertexBuffer];
  uint32 stride_[kMaxVertexBuffer];
  uint32 offset_[kMaxVertexBuffer];
  DISALLOW_COPY_AND_ASSIGN(D3DVertexBufferGroup);
};
}  // namespace d3d11
}  // namespace azer
