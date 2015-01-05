#pragma once

#include "azer/render_system/d3d11/angle_def.h"


// function pointer
extern FUNCGetD3D11Device*        pfnGetDevice;
extern FUNCGetD3D11DeviceContext* pfnGetDeviceContext;
extern FUNCSetSwapRectHook*       pfnSetSwapRectHook;
extern FUNCSetSwapChainResetHook* pfnSetSwapChainResetHook;
extern FUNCGLGETTEXSHARED3DTEX    pfnGetTexShareD3DTex;

bool InitANGLEInterface();
