#pragma once

#include "base/compiler_specific.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/geometry/rect.h"
#include "azer/ui/win/window.h"
#include "azer/ui/win/window_delegate.h"

namespace azer {
namespace win {

Window* CreateTestWindowWithId(int id, Window* parent);
Window* CreateTestWindowWithBounds(const gfx::Rect& bounds, Window* parent);
Window* CreateTestWindow(SkColor color,
                         int id,
                         const gfx::Rect& bounds,
                         Window* parent);
Window* CreateTestWindowWithDelegate(WindowDelegate* delegate,
                                     int id,
                                     const gfx::Rect& bounds,
                                     Window* parent);
Window* CreateTestWindowWithDelegateAndType(WindowDelegate* delegate,
                                            ui::wm::WindowType type,
                                            int id,
                                            const gfx::Rect& bounds,
                                            Window* parent);

}  // nnamespace win
}  // namespace azer
