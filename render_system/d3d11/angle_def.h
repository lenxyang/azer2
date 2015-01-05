#pragma once

#include "GLES2/gl2.h"
#include <d3d11.h>

extern "C" {

// get texture's shared handle
typedef void (*FUNCGetTexShareD3DTex)(GLenum target, GLuint fbhandle, void** val);


// get ANGLE's Direct3D11Device Pointer
typedef void* (*FUNCGetD3D11Device)();

// get ANGLE's Direct3D11DeviceContext Pointer
typedef void* (*FUNCGetD3D11DeviceContext)();


// set a function, which will be called on swap
typedef void (*FUNCSwapRectHook)(ID3D11ShaderResourceView* angle_target,
                                 ID3D11ShaderResourceView** todraw);
typedef void (*FUNCSetSwapRectHook)(FUNCSwapRectHook* hook);

// set a function, which will be called on swapchain reset
typedef void (*FUNCOnSwapChainReset)();
typedef void (*FUNCSetSwapChainResetHook)(FUNCOnSwapChainReset* func);

// set a function, which will be called on swapchain size  
typedef void (*FUNCOnSwapChainResize)();
typedef void (*FUNCSetSwapChainResizeHook)(FUNCOnSwapChainResize* func);
}
