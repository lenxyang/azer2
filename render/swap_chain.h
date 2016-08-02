#pragma once

#include <memory>

#include "base/logging.h"
#include "azer/base/export.h"
#include "azer/render/renderer.h"
#include "azer/render/render_target.h"
#include "azer/render/depth_buffer.h"
#include "azer/render/texture.h"

namespace azer {

class Surface;

class AZER_EXPORT SwapChain : public ::base::RefCounted<SwapChain> {
 public:
  SwapChain() {}
  virtual ~SwapChain() {}

  virtual void SetName(const std::string& name) = 0;
  virtual bool resize(Surface* surface) = 0;
  virtual bool reset(Surface* surface) = 0;
  virtual bool Present() = 0;

  RendererPtr& GetRenderer() { return renderer_;}
 protected:
  RendererPtr renderer_;
  DISALLOW_COPY_AND_ASSIGN(SwapChain);
};

typedef scoped_refptr<SwapChain> SwapChainPtr;
}  // namespace azer

