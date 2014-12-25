#pragma once


#include <d3d11.h>

#include "azer/render/indices_buffer.h"
#include "azer/render_system/d3d11/util.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;

class D3DIndicesBuffer : public IndicesBuffer {
 public:
  virtual ~D3DIndicesBuffer() {
    SAFE_RELEASE(buffer_);
  }

  virtual HardwareBufferDataPtr map(MapType flags) override;
  virtual void unmap() override;

  bool Initialized() { return NULL != buffer_;}
 private:
  D3DIndicesBuffer(const IndicesBuffer::Options& opt, D3DRenderSystem* rs)
      : IndicesBuffer(opt)
      , locked_(false)
      , buffer_(NULL)
      , render_system_(rs) {
  }

  bool Init(const IndicesData* data);
  bool locked_;
  ID3D11Buffer* buffer_;
  D3DRenderSystem* render_system_;

  friend class D3DRenderSystem;
  friend class D3DRenderer;
  DISALLOW_COPY_AND_ASSIGN(D3DIndicesBuffer);
};
}  // namespace d3d11
}  // namespace azer
