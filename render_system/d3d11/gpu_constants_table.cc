#include "azer/render_system/d3d11/gpu_constants_table.h"

#include "base/logging.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/util.h"

namespace azer {
namespace d3d11 {
bool D3DGpuConstantsTable::Init(D3DRenderSystem* rs) {
  D3D11_BUFFER_DESC cbbd;
  ID3D11Device* d3d_device = rs->GetDevice();

  ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
  cbbd.Usage = D3D11_USAGE_DYNAMIC;
  cbbd.ByteWidth = size();
  cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  cbbd.MiscFlags = 0;

  HRESULT hr = d3d_device->CreateBuffer(&cbbd, NULL, &buffer_);
  HRESULT_HANDLE(hr, ERROR, "CreateBuffer for Constants failed ");

  return true;
}

void D3DGpuConstantsTable::flush(Renderer* renderer) {
  DCHECK(buffer_ != NULL);
  ID3D11DeviceContext* d3d_context = ((D3DRenderer*)renderer)->GetContext();
  D3D11_MAPPED_SUBRESOURCE mapped;
  HRESULT hr = d3d_context->Map(buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to map GpuConstantsTable.";
    return;
  }

  memcpy(mapped.pData, data_.get(), size());
  d3d_context->Unmap(buffer_, 0);
  // d3d_context->UpdateSubresource(buffer_, 0, NULL, data_.get(), 0, 0);
}

}  // namespace d3d11
}  // namespace azer
