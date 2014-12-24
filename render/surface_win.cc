#include "azer/render/surface.h"

#include <windows.h>

namespace azer {
void Surface::UpdateBounds() {
  if (type() == kViewSurface) {
    ::RECT rc;
    ::GetClientRect((HWND)GetWindow(), &rc);
    bounds_ = gfx::Rect(0, 0, rc.right - rc.left, rc.bottom - rc.top);
  }
}

}  // namespace azer
