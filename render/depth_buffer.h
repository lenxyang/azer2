#pragma once

#include <memory>
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/texture.h"

namespace azer {

class AZER_EXPORT DepthStencilState : public ::base::RefCounted<DepthStencilState> {
 public:
  // depth buffer state
  virtual void EnableDepthTest(bool enable) = 0;
  virtual bool IsDepthTestEnabled()  = 0;
  virtual void SetDepthCompareFunc(CompareFunc::Type func) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN();
};

typedef scoped_refptr<DepthStencilState> DepthStencilStatePtr;

class AZER_EXPORT DepthBuffer : public ::base::RefCounted<DepthBuffer> {
 public:
  virtual ~DepthBuffer() {};


  Texture* GetTexture() { return texture_.get();}
  enum ClearFlag {
    kClearDepth    = 0x0001,
    kClearStencil  = 0x0002,
    kClearAll      = (kClearDepth | kClearStencil),
  };
 protected:
  DepthBuffer(const Texture::Options& opt)
      : options_(opt) {
  }

  TexturePtr texture_;
  const Texture::Options options_;
  DISALLOW_COPY_AND_ASSIGN(DepthBuffer);
};

typedef scoped_refptr<DepthBuffer> DepthBufferPtr;
}  // namespace azer
