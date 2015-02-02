#include "azer/render_system/d3d11/d3denv.h"

#include <windows.h>

#include "azer/render/surface.h"
#include "azer/render_system/d3d11/util.h"

namespace azer {
namespace d3d11 {

D3DSwapChainEnv::D3DSwapChainEnv(Surface* surface) 
    : d3d_context_(NULL)
    , swap_chain_(NULL)
    , surface_(surface) {
}

D3DSwapChainEnv::D3DSwapChainEnv() {
  SAFE_RELEASE(d3d_context_);
  SAFE_RELEASE(swap_chain_);
}

D3DEnvironment::D3DEnvironment(Surface* surface) 
    : d3d_device_(NULL)
    , dxgi_factory_(NULL)
    , dxgi_adapter_(NULL) {
  memset(&feature_level_, 0, sizeof(feature_level_));
}

D3DEnvironment::~D3DEnvironment() {
  SAFE_RELEASE(d3d_device_);
  SAFE_RELEASE(dxgi_factory_);
  SAFE_RELEASE(dxgi_adapter_);
}

bool D3DEnvironment::InitDXGI() {
  HRESULT hr = S_OK;
  IDXGIDevice *dxgi_device = NULL; 
  hr = d3d_device_->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to get Interface: IDXGIDevice";
    return false;
  }

  hr = dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter_);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to get Interface: IDXGIAdapter";
    return false;
  }

  hr = dxgi_adapter_->GetParent(__uuidof(IDXGIFactory), (void**)&dxgi_factory_);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to get Interface: IDXGIFactory";
    return false;
  }

  return true;
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


