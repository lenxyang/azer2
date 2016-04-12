#include "azer/render_system/d3d11/d3denv.h"

#include <windows.h>

#include "azer/render/surface.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
D3DEnvironment::D3DEnvironment() 
    : d3d_device_(NULL)
    , d3d_context_(NULL)
    , dxgi_factory_(NULL)
    , dxgi_adapter_(NULL) {
  memset(&feature_level_, 0, sizeof(feature_level_));
}

D3DEnvironment::~D3DEnvironment() {
  SAFE_RELEASE(d3d_device_);
  SAFE_RELEASE(d3d_context_);
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

void D3DEnvironment::GetD3DMultisampleSupported(
    std::vector<RenderSystemCapability::SampleDesc>* supported) {
  
  HRESULT hr = 0;
  for (int i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 2) {
    UINT count = static_cast<UINT>(-1);
    hr = d3d_device_->CheckMultisampleQualityLevels(
        DXGI_FORMAT_R8G8B8A8_UNORM, i, &count);

    RenderSystemCapability::SampleDesc desc;
    for (int j = 0; j < count; ++j) {
      desc.count = i;
      desc.quality = j + 1;
      supported->push_back(desc);
    }
  }
}


D3DEnvSwapChain::D3DEnvSwapChain(D3DEnvironment* env, Surface* surface) 
    : swap_chain_(NULL)
    , d3denv_(env)
    , surface_(surface) {
}

D3DEnvSwapChain::~D3DEnvSwapChain() {
  SAFE_RELEASE(swap_chain_);
}

ID3D11Texture2D* D3DEnvSwapChain::GetSwapTexture() {
  HRESULT hr = 0;
  ID3D11Texture2D* texture_buffer = NULL;
  hr = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&texture_buffer);
  if (FAILED(hr)) {
    return NULL;
  }
  return texture_buffer;
}

bool D3DEnvSwapChain::ResetSwapChain() {
  HRESULT hr = 0;
  int32 width = surface_->GetBounds().width();
  int32 height = surface_->GetBounds().height();
  SAFE_RELEASE(swap_chain_);
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
  swapChainDesc.SampleDesc.Count = surface_->sample_desc().count;
  swapChainDesc.SampleDesc.Quality = surface_->sample_desc().quality;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.BufferCount = 1;
  swapChainDesc.OutputWindow = (HWND)surface_->GetWindow();
  swapChainDesc.Windowed = !surface_->fullscreen();
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  IDXGIFactory* factory = d3denv_->GetDxgiFactory();
  hr = factory->CreateSwapChain(d3denv_->GetDevice(), &swapChainDesc, &swap_chain_);
  HRESULT_HANDLE(hr, ERROR, "Failed to create D3D11 and Swapchain ");

  return true;
}
}  // namespace d3d11
}  // namespace azer


