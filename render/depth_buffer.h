#pragma once

#include <memory>
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/texture.h"
#include "azer/render/renderer.h"

namespace azer {

struct DepthStencilOperStruct {
  DepthStencilOper stencil_failed_op;
  DepthStencilOper stencil_pass_op;
  DepthStencilOper depth_pass_op;
  CompareFunc stencil_func;
};

class AZER_EXPORT DepthStencilState : public ::base::RefCounted<DepthStencilState> {
 public:
  DepthStencilState() {}
  virtual ~DepthStencilState() {}
  // depth buffer state
  virtual void EnableDepthTest(bool enable) = 0;
  virtual bool IsDepthTestEnabled() = 0;
  virtual void SetDepthCompareFunc(CompareFunc::Type func) = 0;
  virtual void Apply(Renderer* renderer) = 0;

  virtual void EnableStencil(bool enable) = 0;
  virtual bool IsStencilTestEnabled() = 0; 
  virtual void SetStencilMask(uint8 read_mask, uint8 write_mask) = 0;
  virtual void SetFrontFaceOper();
  virtual void SetBackFaceOper();
 private:
  DISALLOW_COPY_AND_ASSIGN(DepthStencilState);
};

typedef scoped_refptr<DepthStencilState> DepthStencilStatePtr;

class AZER_EXPORT DepthBuffer : public ::base::RefCounted<DepthBuffer> {
 public:
  virtual ~DepthBuffer() {};


  Texture* GetTexture() { return texture_.get();}
 protected:
  DepthBuffer(const Texture::Options& opt) : options_(opt) {}
  TexturePtr texture_;
  const Texture::Options options_;
  DISALLOW_COPY_AND_ASSIGN(DepthBuffer);
};

typedef scoped_refptr<DepthBuffer> DepthBufferPtr;

class ScopedDepthStencilState {
 public:
  ScopedDepthStencilState(Renderer* renderer, DepthStencilState* new_state)
      : renderer_(renderer) {
    prev_state_ = renderer->GetDepthStencilState();;
    renderer->SetDepthStencilState(new_state);
  }
  ~ScopedDepthStencilState() {
    renderer_->SetDepthStencilState(prev_state_);
  }
 private:
  DepthStencilStatePtr prev_state_;
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(ScopedDepthStencilState);
};
}  // namespace azer
