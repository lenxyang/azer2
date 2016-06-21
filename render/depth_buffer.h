#pragma once

#include <memory>

#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/texture.h"
#include "azer/render/resource_view.h"
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
  virtual void SetDepthWriteMask(uint32_t mask) = 0;
  virtual void SetDepthCompareFunc(CompareFunc func) = 0;
  // stencil
  virtual void EnableStencil(bool enable) = 0;
  virtual bool IsStencilTestEnabled() = 0; 
  virtual void SetStencilMask(uint8_t read_mask, uint8_t write_mask) = 0;
  virtual void SetFrontFaceOper(const StencilOperStruct& oper) = 0;
  virtual void SetBackFaceOper(const StencilOperStruct& oper) = 0;
  virtual void Apply(Renderer* renderer, uint32_t stencilref) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(DepthStencilState);
};

typedef scoped_refptr<DepthStencilState> DepthStencilStatePtr;

class AZER_EXPORT DepthBuffer : public ResourceView {
 public:
  struct AZER_EXPORT Options {
    TexType   type;
    TexFormat format;
    bool depth_readonly;
    bool stencil_readonly;

    Options();
  };

  DepthBuffer(const Options& opt);
  virtual ~DepthBuffer();

  const gfx::Size& size() const; 
  Texture* GetTexture() { return texture_.get();}
  const Texture* GetTexture() const { return texture_.get();}
  const Options& options() const { return options_;}
 protected:
  TexturePtr texture_;
  const Options options_;
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
    renderer_->SetDepthStencilState(prev_state_.get(), stencilref_);
  }
 private:
  DepthStencilStatePtr prev_state_;
  Renderer* renderer_;
  uint32_t stencilref_;
  DISALLOW_COPY_AND_ASSIGN(ScopedDepthStencilState);
};
}  // namespace azer
