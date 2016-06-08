#include "azer/render_system/d3d11/angle_d3denv.h"

#include "EGL/egl.h"

#include <windows.h>

#include "azer/render_system/d3d11/angle_env.h"
#include "azer/render_system/d3d11/angle_swapchain.h"
#include "azer/render_system/d3d11/angle_interface.h"
#include "azer/render/surface.h"


namespace azer {
namespace d3d11 {

AngleD3DEnvironment::AngleD3DEnvironment()
    : D3DEnvironment() {
}

bool AngleD3DEnvironment::Initialize() {
  /*
  if (!AngleEnv::Pointer()->InitForView(surface_)) {
    LOG(ERROR) << "Failed to initialize ANGLE.";
  }

  d3d_device_ = (ID3D11Device*)(*pfnGetDevice)();
  d3d_context_ = (ID3D11DeviceContext*)(*pfnGetDeviceContext)();

  if (!InitDXGI()) {
    return false;
  }

  */
  return true;
}

D3DEnvSwapChain* AngleD3DEnvironment::CreateSwapChain(Surface* surface) {
  return NULL;
}
}  // namespace d3d11
}  // namespace azer

