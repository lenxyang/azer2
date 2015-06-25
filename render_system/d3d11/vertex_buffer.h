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
  
  bool Init(const VertexData* dataptr);

  /**
   * 对于 VertexBuffer 来说，它是不分行和列的， 仅仅知道大小
   */
  virtual HardwareBufferDataPtr map(MapType flags) override;
  virtual void unmap() override;

  bool Initialized() const { return NULL != buffer_;}
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
  DISALLOW_COPY_AND_ASSIGN(D3DVertexBuffer);
};

}  // namespace d3d11
}  // namespace azer
