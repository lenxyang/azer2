#pragma once

#include <memory>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

#include "base/basictypes.h"
#include "azer/render/surface.h"
#include "ui/gfx/native_widget_types.h"

namespace azer {

class D3D11Environment;
typedef std::shared_ptr<D3D11Environment> D3D11EnvironmentPtr;

class D3D11Environment {
 public:
  virtual ~D3D11Environment();
  static D3D11EnvironmentPtr Create(const std::string& name, Surface* surface);

  /**
   * direct3d relevent
   */
  ID3D11Device* GetDevice() { return d3d_device_;}
  ID3D11DeviceContext* GetContext() { return d3d_context_;}
  IDXGIFactory* GetDxgiFactory() { return dxgi_factory_;}
  IDXGISwapChain* GetSwapChain() { return swap_chain_;}
  const D3D_FEATURE_LEVEL&  feature_level() const { return feature_level_;}

  ID3D11Texture2D* GetSwapTexture();

  virtual bool ResetSwapChain() = 0;
  Surface* GetSurface() { return surface_;}
 protected:
  D3D11Environment(Surface* ptr);
  
  virtual bool Initialize() = 0;

  ID3D11Device* d3d_device_;
  ID3D11DeviceContext* d3d_context_;
  IDXGIFactory* dxgi_factory_;
  IDXGIAdapter* dxgi_adapter_;
  IDXGISwapChain* swap_chain_;
  D3D_FEATURE_LEVEL feature_level_;
  Surface* surface_;
  DISALLOW_COPY_AND_ASSIGN(D3D11Environment);
};

}  // namespace azer
