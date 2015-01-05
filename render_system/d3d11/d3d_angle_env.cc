#include "azer/render_system/d3d11/d3d_angle_env.h"
#include "azer/render_system/d3d11/angle_swapchain.h"

#include "EGL/egl.h"

#include <windows.h>
#include <d3d11.h>

extern "C" {
typedef void* (*FUNCGetD3D11Device)();
typedef void* (*FUNCGetD3D11DeviceContext)();

typedef void (*FUNCSwapRectHook)(ID3D11Texture2D* angle_target,
                                 ID3D11Texture2D* todraw);
typedef void (*FUNCSetSwapRectHook)(FUNCSwapRectHook* hook);

typedef void (*FUNCOnSwapChainReset)();
typedef void (*FUNCSetSwapChainResetHook)(FUNCOnSwapChainReset* func);
}

namespace {
const char* funcname_GetDevice = "GetD3D11DeviceANGLE";
const char* funcname_GetContext = "GetD3D11DeviceContextANGLE";
const char* funcname_SetSwapChainHook = "SetSwapChainHookANGLE";
const char* funcname_SetSwapChainResetHook = "SetSwapChainResetHookANGLE";
FUNCGetD3D11Device*        pfnGetDevice = NULL;
FUNCGetD3D11DeviceContext* pfnGetDeviceContext = NULL;
FUNCSetSwapRectHook*       pfnSetSwapRectHook = NULL;
FUNCSetSwapChainResetHook* pfnSetSwapChainResetHook = NULL;
}

namespace azer {
namespace d3d11 {

AngleD3DEnvironment::AngleD3DEnvironment(Surface* surface)
    : D3DEnvironment(surface) {
}

bool AngleD3DEnvironment::ResetSwapChain() {
  CHECK(false);
}

void AngleD3DEnvironment::InitFuncProc() {
  static bool initialized = false;
  if (!initialized) {
    initialized = true;

    
  } else {
    return true;
  }
}

bool AngleD3DEnvironment::Initialize() {
  if (!AngleEnv::Pointer()->InitForView(surface)) {
    LOG(ERROR) << "Failed to initialize ANGLE.";
  }

  if (!InitFuncProc()) {
    return false;
  }

  d3d_device_ = (*pfnGetDevice)();
  d3d_context_ = (*pfnGetDeviceContext)();

  if (!InitDXGI()) {
    return false;
  }

  return true;
}

SwapChain* AngleD3DEnvironment::CreateSwapChain(D3DRenderSystem* rs) {
}
}  // namespace d3d11
}  // namespace azer

