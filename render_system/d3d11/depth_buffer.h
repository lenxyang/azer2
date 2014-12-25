#pragma once

#include <d3d11.h>
#include <stack>

#include "base/logging.h"
#include "azer/render/depth_buffer.h"
#include "azer/render_system/d3d11/util.h"

namespace azer {

class Surface;

namespace d3d11 {
class D3DRenderSystem;
class D3DRenderer;

class D3DDepthBuffer : public DepthBuffer {
 public:
  static D3DDepthBuffer* Create(Surface* surface, D3DRenderer* renderer);
  static D3DDepthBuffer* Create(const Texture::Options& opt, 
                                  D3DRenderer* renderer);
   // create by rendersystem
  D3DDepthBuffer(const Texture::Options& opt, D3DRenderer* renderer)
      : DepthBuffer(opt)
      , stencil_ref_value_(0)
      , target_(NULL)
      , renderer_(renderer) {
  }

  virtual ~D3DDepthBuffer() {
    SAFE_RELEASE(target_);
  }
  
  virtual void Enable(bool enable);
  virtual bool IsEnabled();

  virtual void SetDepthCompareFunc(CompareFunc::Type func);

  virtual void EnableWrite(bool) {CHECK(false);}
  virtual void SetBias(float val) {CHECK(false);}

  virtual void PushState();
  virtual void PopState();

  bool Init(D3DRenderSystem* rs);
  void Clear(D3DRenderer*, ClearFlag flag = kClearAll, float depth_val = 1.0,
             int stencil_val = 0);

  ID3D11DepthStencilView* GetD3DDepthStencilView() { return target_;}
 private:
  void UpdateState();
  bool InitDepthAndStencilState(D3DRenderSystem* rs);

  std::stack<D3D11_DEPTH_STENCIL_DESC> state_stack_;

  int32 stencil_ref_value_;
  ID3D11DepthStencilView* target_;
  D3DRenderer* renderer_;
  D3D11_DEPTH_STENCIL_DESC desc_;
  friend class D3DRenderer;
  friend class D3DRenderSystem;
  DISALLOW_COPY_AND_ASSIGN(D3DDepthBuffer);
};

}  // namespace d3d11
}  // namespace azer
