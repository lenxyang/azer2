#pragma once

#include <d3d11.h>

#include "base/logging.h"
#include "azer/render/depth_buffer.h"

namespace azer {

class Surface;

namespace d3d11 {
class D3DRenderSystem;
class D3DRenderer;

class D3DDepthBuffer : public DepthBuffer {
 public:
  static D3DDepthBuffer* Create(Surface* surface, D3DRenderSystem* rs);
  static D3DDepthBuffer* Create(const Texture::Options& opt, 
                                D3DRenderSystem* rs);
   // create by rendersystem
  D3DDepthBuffer(const Texture::Options& opt, D3DRenderSystem* rs);
  ~D3DDepthBuffer() override;

  bool Init(D3DRenderSystem* rs);
  void Clear(D3DRenderer*, ClearFlag flag = kClearAll, float depth_val = 1.0,
             int stencil_val = 0);

  ID3D11DepthStencilView* GetD3DDepthStencilView() { return target_;}
 private:
  ID3D11DepthStencilView* target_;
  D3DRenderSystem* render_system_;
  friend class D3DRenderSystem;
  DISALLOW_COPY_AND_ASSIGN(D3DDepthBuffer);
};

}  // namespace d3d11
}  // namespace azer
