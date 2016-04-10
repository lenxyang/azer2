#pragma once

#include <memory>
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/texture.h"
#include "azer/render/renderer.h"

namespace azer {

struct StencilOperStruct {
  StencilOper failed_op;
  StencilOper pass_op;
  StencilOper depth_failed_op;
  CompareFunc stencil_func;
};

class AZER_EXPORT DepthStencilState : public ::base::RefCounted<DepthStencilState> {
 public:
  DepthStencilState();
  virtual ~DepthStencilState();
  // depth buffer state
  virtual void EnableDepthTest(bool enable) = 0;
  virtual bool IsDepthTestEnabled() = 0;
  virtual void SetDepthWriteMask(uint32 mask) = 0;
  virtual void SetDepthCompareFunc(CompareFunc func) = 0;
  // stencil
  virtual void EnableStencil(bool enable) = 0;
  virtual bool IsStencilTestEnabled() = 0; 
  virtual void SetStencilMask(uint8 read_mask, uint8 write_mask) = 0;
  virtual void SetFrontFaceOper(const StencilOperStruct& oper) = 0;
  virtual void SetBackFaceOper(const StencilOperStruct& oper) = 0;
  virtual void Apply(Renderer* renderer, uint32 stencilref) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(DepthStencilState);
};

typedef scoped_refptr<DepthStencilState> DepthStencilStatePtr;

class AZER_EXPORT DepthBuffer : public ::base::RefCounted<DepthBuffer> {
 public:
  virtual ~DepthBuffer() {};

  const gfx::Size& size() const; 
  Texture* GetTexture() { return texture_.get();}
  Texture* GetResTexture() { return restexture_.get();}
 protected:
  DepthBuffer(const Texture::Options& opt) : options_(opt) {}
  TexturePtr texture_;
  TexturePtr restexture_;
  const Texture::Options options_;
  DISALLOW_COPY_AND_ASSIGN(DepthBuffer);
};

typedef scoped_refptr<DepthBuffer> DepthBufferPtr;

class ScopedDepthStencilState {
 public:
  ScopedDepthStencilState(Renderer* renderer)
      : renderer_(renderer) {
    prev_state_ = renderer->GetDepthStencilState();;
    stencilref_ = renderer->stencilref();
  }
  ~ScopedDepthStencilState() {
    renderer_->SetDepthStencilState(prev_state_, stencilref_);
  }
 private:
  DepthStencilStatePtr prev_state_;
  Renderer* renderer_;
  uint32 stencilref_;
  DISALLOW_COPY_AND_ASSIGN(ScopedDepthStencilState);
};
}  // namespace azer
