#pragma once

#include "ui/gfx/geometry/rect.h"

#include "azer/base/export.h"
#include "azer/base/render_loop.h"
#include "azer/render/render.h"

namespace azer {
namespace util {
class AZER_EXPORT RenderWindow {
 public:
  RenderWindow(const gfx::Rect& bounds);
  ~RenderWindow();

  void Loop(RenderLoop::Delegate* delegate);
 private:
  const gfx::Rect bounds_;
  DISALLOW_COPY_AND_ASSIGN(RenderWindow);
};
}  // namespace util
}  // namespace azer
