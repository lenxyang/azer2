#pragma once

#include <memory>

#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/texture.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/native_widget_types.h"

namespace azer {

class SwapChain;

/**
 * Surface 类似于 egl Surface， 在 SwapBuffer 之后最终结果展示与此
 */
class AZER_EXPORT Surface : public ::base::RefCounted<Surface> {
 public:
  enum Type {
    kViewSurface,
    kOffscreenSurface,
  };

  Surface(gfx::AcceleratedWidget window) 
      : type_(kViewSurface)
      , fullscreen_(false)
      , swapchain_(NULL)
      , window_(window) {
    UpdateBounds();
  }

  Surface(const gfx::Rect& rect)
      : type_(kOffscreenSurface)
      , fullscreen_(false)
      , swapchain_(NULL)
      , bounds_(rect)
      , window_(0) {
  }

  const SampleDesc& sample_desc() const { return sample_desc_;}
  void SetExpectedSampleDesc(const SampleDesc& desc) {
    sample_desc_ = desc;
  }

  const gfx::Rect& GetBounds() { return bounds_;}
  gfx::AcceleratedWidget GetWindow() { return window_;}

  Type type() const { return type_;}
  bool fullscreen() const { return fullscreen_;}

  SwapChain* GetSwapChain() { return swapchain_;}

  // dont call it
  void SetSwapChain(SwapChain* swapchain) {swapchain_ = swapchain;}
 protected:
  void UpdateBounds();

  Type type_;
  bool fullscreen_;
  SwapChain* swapchain_;
  gfx::Rect bounds_;
  gfx::AcceleratedWidget window_;
  SampleDesc sample_desc_;
  DISALLOW_COPY_AND_ASSIGN(Surface);
};

typedef scoped_refptr<Surface> SurfacePtr;
}  // namespace azer
