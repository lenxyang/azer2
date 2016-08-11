#pragma once

#include "azer/render/common.h"
#include "azer/render/vertex_layout.h"
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
}  // namespace d3d11
}  // namespace azer
