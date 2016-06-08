#pragma once

#include "azer/render/indices_buffer.h"
#include "azer/render_system/d3d11/d3d11.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;

class D3DIndicesBuffer : public IndicesBuffer {
 public:
  D3DIndicesBuffer(const HBufferOptions& opt, D3DRenderSystem* rs);
  ~D3DIndicesBuffer() override;

  HardwareBufferDataPtr map(MapType flags) override;
  void unmap() override;

  bool Initialized() { return NULL != buffer_;}
  ID3D11Buffer* buffer() { return buffer_;}
 private:
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
