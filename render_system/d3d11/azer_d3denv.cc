#include "azer/render_system/d3d11/azer_d3denv.h"

#include <windows.h>

#include "azer/render/surface.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/angle_env.h"
#include "azer/render_system/d3d11/swap_chain.h"

namespace azer {
namespace d3d11 {

InternalD3DEnvironment::InternalD3DEnvironment()
    : D3DEnvironment() {
}

bool InternalD3DEnvironment::InitD3DDevice() {
  DCHECK(NULL == d3d_device_);
  DCHECK(NULL == dxgi_adapter_);
  DCHECK(NULL == dxgi_factory_);
  // Create our SwapChain
  HRESULT hr;
  D3D_FEATURE_LEVEL featureLevels[] = {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
  };

  hr = D3D11CreateDevice(NULL,
                         D3D_DRIVER_TYPE_HARDWARE,
                         NULL,
                         NULL,
                         featureLevels, arraysize(featureLevels),
                         D3D11_SDK_VERSION,
                         &d3d_device_,
                         &feature_level_,
                         &d3d_context_);
  if (FAILED(hr)) {
    return false;
  }

  if (!InitDXGI()) {
    return false;
  }

  return true;
}

bool InternalD3DEnvironment::Initialize() {
  if (!InitD3DDevice()) {
    return false;
  }

  return true;
}

D3DEnvSwapChain* InternalD3DEnvironment::CreateSwapChain(Surface* surface) {
  std::unique_ptr<D3DEnvSwapChain> ptr(new D3DEnvSwapChain(this, surface));
  if (!ptr->ResetSwapChain()) {
    return NULL;
  }

  return ptr.release();
}

}  // namespace d3d11
}  // namespace azer
