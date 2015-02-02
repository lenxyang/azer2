#pragma once

#include <memory>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

#include "base/basictypes.h"
#include "azer/render/surface.h"
#include "azer/render/swap_chain.h"
#include "ui/gfx/native_widget_types.h"

namespace azer {
namespace d3d11 {

class D3DSwapChain;
class D3DRenderSystem;
class D3DEnvironment;

typedef std::shared_ptr<D3DEnvironment> D3DEnvironmentPtr;

class D3DSwapChainEnv {
 public:
  D3DSwapChainEnv(Surface* ptr);

  virtual ~D3DSwapChainEnv();

  ID3D11DeviceContext* GetContext() { return d3d_context_;}
  IDXGISwapChain* GetSwapChain() { return swap_chain_;}

  ID3D11Texture2D* GetSwapTexture();

  virtual bool ResetSwapChain() = 0;
  Surface* GetSurface() { return surface_;}
 private:
  ID3D11DeviceContext* d3d_context_;
  IDXGISwapChain* swap_chain_;
  Surface* surface_;
  DISALLOW_COPY_AND_ASSIGN(D3DSwapChainEnv);
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
  virtual SwapChain* CreateSwapChain(D3DRenderSystem* rs, Surface* surface) = 0;
  /**
   * direct3d relevent
   */
  ID3D11Device* GetDevice() { return d3d_device_;}
  IDXGIFactory* GetDxgiFactory() { return dxgi_factory_;}
  const D3D_FEATURE_LEVEL&  feature_level() const { return feature_level_;}
 protected:
  D3DEnvironment(Surface* ptr);

  // after get D3D11Device, get dxgi interface pointer
  bool InitDXGI();

  IDXGIFactory* dxgi_factory_;
  IDXGIAdapter* dxgi_adapter_;
  ID3D11Device* d3d_device_;
  D3D_FEATURE_LEVEL feature_level_;
  DISALLOW_COPY_AND_ASSIGN(D3DEnvironment);
};

}  // namespace d3d11
}  // namespace azer
