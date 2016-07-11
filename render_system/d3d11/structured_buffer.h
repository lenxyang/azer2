#pragma once

#include "azer/render_system/d3d11/d3d11.h"
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
  std::unique_ptr<GpuBufferMapHelper> map_helper_;
  DISALLOW_COPY_AND_ASSIGN(D3DStructuredGpuBuffer);
};

class D3DStructuredGpuBufferView: public StructuredGpuBufferView {
 public:
  D3DStructuredGpuBufferView(const Options& opt, StructuredGpuBuffer* buffer);
  ~D3DStructuredGpuBufferView() override;

  bool Init(D3DRenderSystem* rs);
  ID3D11ShaderResourceView* GetResourceView() { return res_view_;}
 private:
  ID3D11ShaderResourceView* res_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DStructuredGpuBufferView);
};

class D3DUAStructuredGpuBufferView: public StructuredGpuBufferView {
 public:
  D3DUAStructuredGpuBufferView(const Options& opt, StructuredGpuBuffer* buffer);
  ~D3DUAStructuredGpuBufferView() override;

  bool Init(D3DRenderSystem* rs);
  ID3D11UnorderedAccessView* GetResourceView() { return unorder_view_;}
 private:
  ID3D11UnorderedAccessView* unorder_view_;
  DISALLOW_COPY_AND_ASSIGN(D3DUAStructuredGpuBufferView);
};
}  // namespace d3d11
}  // namespace azer
