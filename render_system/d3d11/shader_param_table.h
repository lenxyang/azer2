#pragma once

#include "base/logging.h"
#include "azer/render/shader_param_table.h"
#include "azer/render_system/d3d11/d3d11.h"

namespace azer {
namespace d3d11 {
class D3DRenderer;
class D3DRenderSystem;

class D3DShaderParamTable : public ShaderParamTable {
 public:
  D3DShaderParamTable(int32_t num, const Desc* desc);
  ~D3DShaderParamTable() override;

  void SetName(const std::string& name) override;
  void flush(Renderer*) override;
  bool CopyTo(GpuResource* res) override;
  NativeGpuResourceHandle native_handle() override {
    return (NativeGpuResourceHandle)buffer_;
  }
 private:
  ID3D11Buffer* buffer_;
  bool Init(D3DRenderSystem* rs);

  friend class D3DRenderSystem;
  friend class D3DRenderer;
  DISALLOW_COPY_AND_ASSIGN(D3DShaderParamTable);
};
}  // namespace d3d11
}  // namespace azer
