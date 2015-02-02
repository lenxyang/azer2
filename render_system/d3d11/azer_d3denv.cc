#include "azer/render_system/d3d11/azer_d3denv.h"

#include <windows.h>

#include "azer/render/surface.h"
#include "azer/render_system/d3d11/util.h"
#include "azer/render_system/d3d11/angle_env.h"
#include "azer/render_system/d3d11/swap_chain.h"

namespace azer {
namespace d3d11 {

InternalD3DEnvironment::InternalD3DEnvironment(Surface* surface)
    : D3DEnvironment(surface) {
}

bool InternalD3DEnvironment::ResetSwapChain() {
  HRESULT hr = 0;
  int32 width = surface_->GetBounds().width();
  int32 height = surface_->GetBounds().height();
  SAFE_RELEASE(swap_chain_);
  DCHECK(NULL != dxgi_factory_);
  //Describe our SwapChain
  DXGI_MODE_DESC bufferDesc;
  ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
  bufferDesc.Width = width;
  bufferDesc.Height = height;
  bufferDesc.RefreshRate.Numerator = 60;
  bufferDesc.RefreshRate.Denominator = 1;
  bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

  DXGI_SWAP_CHAIN_DESC swapChainDesc; 
  ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
  swapChainDesc.BufferDesc = bufferDesc;
  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.SampleDesc.Quality = 0;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.BufferCount = 1;
  swapChainDesc.OutputWindow = (HWND)surface_->GetWindow();
  swapChainDesc.Windowed = !surface_->fullscreen();
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  IDXGIFactory* factory = dxgi_factory_;
  hr = factory->CreateSwapChain(d3d_device_, &swapChainDesc, &swap_chain_);
  HRESULT_HANDLE(hr, ERROR, "Failed to create D3D11 and Swapchain ");

  return true;
}

bool InternalD3DEnvironment::InitD3DDevice() {
  DCHECK(NULL == d3d_device_);
  DCHECK(NULL == d3d_context_);
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

  ResetSwapChain();

  return true;
}

SwapChain* InternalD3DEnvironment::CreateSwapChain(D3DRenderSystem* rs, 
                                                   Surface* surface) {
  std::unique_ptr<D3DSwapChain> ptr(new D3DSwapChain(rs));
  if (!ptr->Init(surface)) {
    return NULL;
  }

  return ptr.release();
}

}  // namespace d3d11
}  // namespace azer
