#pragma once

#include <memory>
#include "base/basictypes.h"
#include "azer/base/export.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/native_widget_types.h"

namespace azer {

/**
 * Surface 类似于 egl Surface， 在 SwapBuffer 之后最终结果展示与此
 */
class AZER_EXPORT Surface {
 public:
  enum Type {
    kViewSurface,
    kOffscreenSurface,
  };

  Surface(gfx::AcceleratedWidget window) 
      : type_(kViewSurface)
      , fullscreen_(false)
      , window_(window)  {
    UpdateBounds();
  }

  Surface(const gfx::Rect& rect)
      : type_(kOffscreenSurface)
      , fullscreen_(false)
      , bounds_(rect)
      , window_(0)  {
  }

  const gfx::Rect& GetBounds() { return bounds_;}
  gfx::AcceleratedWidget GetWindow() { return window_;}

  Type type() const { return type_;}
  bool fullscreen() const { return fullscreen_;}
 protected:
  void UpdateBounds();

  Type type_;
  bool fullscreen_;
  gfx::Rect bounds_;
  gfx::AcceleratedWidget window_;
  DISALLOW_COPY_AND_ASSIGN(Surface);
};

typedef std::shared_ptr<Surface> SurfacePtr;
}  // namespace azer
