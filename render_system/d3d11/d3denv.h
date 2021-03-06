#pragma once

#include <memory>

#include "azer/render/surface.h"
#include "azer/render/swap_chain.h"
#include "ui/gfx/native_widget_types.h"
#include "azer/render/render_capability.h"
#include "azer/render_system/d3d11/d3d11.h"

namespace azer {
namespace d3d11 {

class D3DSwapChain;
class D3DRenderSystem;
class D3DEnvironment;

typedef std::shared_ptr<D3DEnvironment> D3DEnvironmentPtr;

class D3DEnvSwapChain {
 public:
  D3DEnvSwapChain(D3DEnvironment* env, Surface* ptr);

  virtual ~D3DEnvSwapChain();

  IDXGISwapChain* GetSwapChain() { return swap_chain_;}

  ID3D11Texture2D* GetSwapTexture();
  virtual bool ResetSwapChain();
  Surface* GetSurface() { return surface_;}
 private:
  IDXGISwapChain* swap_chain_;
  D3DEnvironment* d3denv_;
  Surface* surface_;
  DISALLOW_COPY_AND_ASSIGN(D3DEnvSwapChain);
};
/**
 * D3DEnviroment 的作用是封装 D3D11 的初始化过程
 * 在内部实现中, D3DEnvironment 分成两个部分 External 和 Internal
 * Internal 是默认的方法，即在内部完成 D3D11 的初始化
 * External 则为外部方法，D3D11 通过外部程序完成初始化，如果希望 azer 与 ANGLE 结合
 * 那么应该使用 ANGLE 的 d3d11 初始化的对象。
 *
 */
class D3DEnvironment {
 public:
  virtual ~D3DEnvironment();

  virtual bool Initialize() = 0;
  virtual D3DEnvSwapChain* CreateSwapChain(Surface* surface) = 0;
  /**
   * direct3d relevent
   */
  ID3D11Device* GetDevice() { return d3d_device_;}
  IDXGIFactory* GetDxgiFactory() { return dxgi_factory_;}
  ID3D11DeviceContext* GetContext() { return d3d_context_;}
  const D3D_FEATURE_LEVEL&  feature_level() const { return feature_level_;}

  void GetD3DMultisampleSupported(
      std::vector<RenderSystemCapability::SampleDesc>* supported);
 protected:
  D3DEnvironment();

  // after get D3D11Device, get dxgi interface pointer
  bool InitDXGI();

  IDXGIFactory* dxgi_factory_;
  IDXGIAdapter* dxgi_adapter_;
  ID3D11Device* d3d_device_;
  ID3D11DeviceContext* d3d_context_;
  D3D_FEATURE_LEVEL feature_level_;
  DISALLOW_COPY_AND_ASSIGN(D3DEnvironment);
};

}  // namespace d3d11
}  // namespace azer
