#include "azer/render_system/d3d11/d3denv.h"

#include <windows.h>

#include "azer/render/surface.h"p
#include "azer/render_system/d3d11/util.h"
#include "azer/render_system/d3d11/d3d_internal_env.h"
#include "azer/render_system/d3d11/d3d_angle_env.h"

namespace azer {
namespace d3d11 {
D3DEnvironment::D3DEnvironment(Surface* surface) 
    : d3d_device_(NULL)
    , d3d_context_(NULL)
    , dxgi_factory_(NULL)
    , dxgi_adapter_(NULL)
    , swap_chain_(NULL)
    , surface_(surface) {
  memset(&feature_level_, 0, sizeof(feature_level_));
}

D3DEnvironment::~D3DEnvironment() {
  SAFE_RELEASE(d3d_device_);
  SAFE_RELEASE(d3d_context_);
  SAFE_RELEASE(dxgi_factory_);
  SAFE_RELEASE(dxgi_adapter_);
  SAFE_RELEASE(swap_chain_);
}

ID3D11Texture2D* D3DEnvironment::GetSwapTexture() {
  HRESULT hr = 0;
  ID3D11Texture2D* texture_buffer = NULL;
  hr = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&texture_buffer);
  if (FAILED(hr)) {
    return NULL;
  }
  return texture_buffer;
}

}  // namespace d3d11
}  // namespace azer


