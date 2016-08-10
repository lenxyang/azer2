#include "azer/render_system/d3d11/shader_param_table.h"

#include "base/logging.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
D3DShaderParamTable::D3DShaderParamTable(int32_t num, const Desc* desc)
    : ShaderParamTable(num, desc)
    , buffer_(NULL) {
}

D3DShaderParamTable::~D3DShaderParamTable() {
  SAFE_RELEASE(buffer_);
}

bool D3DShaderParamTable::Init(D3DRenderSystem* rs) {
  D3D11_BUFFER_DESC cbbd;
  ID3D11Device* d3d_device = rs->GetDevice();
  
  ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
  cbbd.Usage = D3D11_USAGE_DEFAULT;
  cbbd.ByteWidth = size();
  cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cbbd.CPUAccessFlags = 0;
  cbbd.MiscFlags = 0;

  HRESULT hr = d3d_device->CreateBuffer(&cbbd, NULL, &buffer_);
  HRESULT_HANDLE(hr, ERROR, "CreateBuffer for Constants failed ");

  return true;
}

void D3DShaderParamTable::SetName(const std::string& name) {
  DCHECK(buffer_);
  buffer_->SetPrivateData(WKPDID_D3DDebugObjectName,
                          (UINT)name.length(), name.c_str());
}

void D3DShaderParamTable::flush(Renderer* renderer) {
  DCHECK(buffer_ != NULL);
  ID3D11DeviceContext* d3d_context = ((D3DRenderer*)renderer)->GetContext();
  // if use map and unmap, usage must be D3D11_USAGE_DYNAMIC and
  // CPUAccessFlags must be CPU_ACCESS_WRITE
  /*
  D3D11_MAPPED_SUBRESOURCE mapped;
  HRESULT hr = d3d_context->Map(buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to map ShaderParamTable.";
    return;
  }

  memcpy(mapped.pData, data_.get(), size());
  d3d_context->Unmap(buffer_, 0);
  */
  d3d_context->UpdateSubresource(buffer_, 0, NULL, data_.get(), 0, 0);
}

bool D3DShaderParamTable::CopyTo(GpuResource* res) {
  CHECK(false);
  return false;
}

}  // namespace d3d11
}  // namespace azer
