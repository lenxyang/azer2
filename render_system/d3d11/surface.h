#pragma once

#include "base/basictypes.h"

#include <windows.h>

#include "azer/render/surface.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/native_widget_types.h"

namespace azer {
class D3D11Surface : public Surface {
 public:
  D3D11Surface(gfx::AcceleratedWidget window) 
      : Surface(window) {
  }

  virtual void UpdateBounds() override {
    ::RECT rc;
    ::GetClientRect((HWND)GetWindow(), &rc);
    bounds_ = gfx::Rect(0, 0, rc.right - rc.left, rc.bottom - rc.top);
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(D3D11Surface);
};
}  // namespace azer
