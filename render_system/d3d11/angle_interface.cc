#include "azer/render_system/d3d11/angle_interface.h"

#include "azer/render_system/d3d11/angle_env.h"

namespace {
const char* fname_GetDevice = "GetD3D11DeviceANGLE";
const char* fname_GetContext = "GetD3D11DeviceContextANGLE";
const char* fname_SetSwapChainHook = "SetSwapChainHookANGLE";
const char* fname_SetSwapChainResetHook = "SetSwapChainResetHookANGLE";
const char* fname_GetTexShareD3DTex = "glGetTexShareD3DTex";
}  // namespace


FUNCGetD3D11Device        pfnGetDevice = NULL;
FUNCGetD3D11DeviceContext pfnGetDeviceContext = NULL;
FUNCSetSwapRectHook       pfnSetSwapRectHook = NULL;
FUNCSetSwapChainResetHook pfnSetSwapChainResetHook = NULL;
FUNCGetTexShareD3DTex     pfnGetTexShareD3DTex = NULL;


namespace azer {
namespace d3d11 {

bool InitANGLEInterface() {
  if (!GetANGLEProcAddress(fname_GetTexShareD3DTex, (void**)&pfnGetTexShareD3DTex)) {
    LOG(ERROR) << "Failed to get address: " << fname_GetTexShareD3DTex;
    return false;
  }

  return true;
}

}  // namespace d3d11
}  // namespace azer
