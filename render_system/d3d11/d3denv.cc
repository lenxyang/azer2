#include "azer/render_system/d3d11/d3denv.h"

#include <windows.h>

#include "azer/render/surface.h"
#include "azer/render_system/d3d11/util.h"

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

class InternalD3DEnvironment : public D3DEnvironment {
 public:
  InternalD3DEnvironment(Surface* surface)
      : D3DEnvironment(surface) {
  }

  virtual bool ResetSwapChain() override;
  virtual bool Initialize() override;
 private:
  bool InitD3DDevice();
  DISALLOW_COPY_AND_ASSIGN(InternalD3DEnvironment);
};

class ExternalD3DEnvironment : public D3DEnvironment {
 public:
  ExternalD3DEnvironment(Surface* surface)
      : D3DEnvironment(surface) {
  }

  virtual bool ResetSwapChain() override {}
  virtual bool Initialize() override;
 private:
  DISALLOW_COPY_AND_ASSIGN(ExternalD3DEnvironment);
};

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

  IDXGIDevice *dxgiDevice = NULL; 
  hr = d3d_device_->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to get Interface: IDXGIDevice";
    return false;
  }

  hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter_);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to get Interface: IDXGIAdapter";
    return false;
  }

  hr = dxgi_adapter_->GetParent(__uuidof(IDXGIFactory), (void**)&dxgi_factory_);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to get Interface: IDXGIFactory";
    return false;
  }

  d3d_context_->AddRef();
  return true;
}

bool InternalD3DEnvironment::Initialize() {
  if (!InitD3DDevice()) {
    return false;
  }

  ResetSwapChain();
  return true;
}


D3DEnvironmentPtr D3DEnvironment::Create(const std::string& name, 
                                             Surface* surface) {
  if (name == "internal") {
    std::unique_ptr<D3DEnvironment> ptr(new InternalD3DEnvironment(surface));
    if (ptr->Initialize()) {
      return D3DEnvironmentPtr(ptr.release());
    }
  } else if (name == "external") {
    return D3DEnvironmentPtr();
  } else {
    NOTREACHED();
  }

  return D3DEnvironmentPtr();
}
}  // namespace d3d11
}  // namespace azer


