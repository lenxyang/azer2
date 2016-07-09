#pragma once

#include "azer/render/structured_buffer.h"

namespace azer {
namespace d3d11 {
class D3DRenderSystem;
class D3DStructuredGpuBuffer : public StructuredGpuBuffer {
 public:
  D3DStructuredGpuBuffer(const GpuBufferOptions& options, int count, int strip);
  ~D3DStructuredGpuBuffer() override;

  GpuBufferDataPtr map(MapType flags) override;
  void unmap() override;
  ID3D11Buffer* object() { return bufobj_;}

  bool Init(D3DRenderSystem* rs);
 private:
  ID3D11Buffer* bufobj_;
  DISALLOW_COPY_AND_ASSIGN(D3DStructuredGpuBuffer);
};

class D3DUAStructuredGpuBufferView: public StructuredGpuBufferView {
 public:
  D3DUAStructuredGpuBufferView(const Options& opt, StructuredGpuBuffer* buffer);
  ~D3DUAStructuredGpuBufferView() override;

  bool Init(D3DRenderSystem* rs);
 private:
  ID3D11UnorderedAccessView* unorder_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DUAStructuredGpuBufferView);
};
}  // namespace d3d11
}  // namespace azer
