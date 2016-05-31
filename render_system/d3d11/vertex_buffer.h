#pragma once


#include "azer/render/vertex_buffer.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
class D3DBlob;

class D3DVertexLayout : public VertexLayout {
 public:
  D3DVertexLayout(VertexDesc* desc);
  ~D3DVertexLayout() override;

  bool Init(RenderSystem* rs) override;
  bool Init(RenderSystem* rs, ID3DBlob* blob);
  ID3D11InputLayout* input_layout() { return input_layout_;}
  bool ValidateShaderLayout(RenderSystem* rs, ID3DBlob* blob);
 private:
  void CreateInputDesc(VertexDesc* desc, D3D11_INPUT_ELEMENT_DESC* d3ddesc);
  std::string GenVSForDesc(VertexDesc* desc);
  ID3D11InputLayout *input_layout_;
  static const int32_t kMaxInputElementDesc = 256;
  DISALLOW_COPY_AND_ASSIGN(D3DVertexLayout);
};

class D3DVertexBuffer : public VertexBuffer {
 public:
  D3DVertexBuffer(const HBufferOptions &opt, D3DRenderSystem* rs);
  virtual ~D3DVertexBuffer();
  bool Init(SlotVertexData* dataptr);

  HardwareBufferDataPtr map(MapType flags) override;
  void unmap() override;

  bool Initialized() const { return NULL != buffer_;}
  ID3D11Buffer* buffer() { return buffer_;}
 private:
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
