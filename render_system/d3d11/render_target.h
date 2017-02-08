#pragma once

#include "azer/render/render_target.h"
#include "azer/render_system/d3d11/d3d11.h"
#include "azer/render_system/d3d11/d3denv.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render_system/d3d11/texture.h"
#include "azer/render_system/d3d11/swap_chain.h"

namespace azer {
namespace d3d11 {
class D3DTexture;
class D3DRenderSystem;
class D3DRenderer;
class D3DSurfaceRenderTarget;

class D3DRenderTarget : public RenderTarget {
 public:
  // create by RenderSystem
  D3DRenderTarget(const Options& opt, bool surface_target, 
                  D3DRenderSystem* render_system);
  virtual ~D3DRenderTarget();
  
  void SetName(const std::string& name) override;
  void Clear(const azer::Vector4& color) override;

  bool Init(D3DTexture2D* tex);
  bool InitFor2DArray(Texture* tex, const Texture2DArraySlice& slice);
  ID3D11RenderTargetView* GetD3DRenderTargetView() { return target_;}
  NativeGpuResourceViewHandle native_handle() override;
 protected:
  ID3D11RenderTargetView* target_;
  ID3D11Resource* texres_;
  D3DRenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(D3DRenderTarget);
};

}  // namespace d3d11
}  // namespace azer
