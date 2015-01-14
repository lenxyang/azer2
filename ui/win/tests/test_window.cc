#include "azer/ui/win/tests/test_window.h"

#include "base/strings/string_number_conversions.h"
#include "ui/gfx/rect.h"

#include "azer/ui/win/client/constants.h"
#include "azer/ui/win/window.h"
#include "azer/ui/win/window_property.h"
#include "azer/ui/win/tests/test_window_delegate.h"


namespace azer {
namespace win {

Window* CreateTestWindowWithId(int id, Window* parent) {
  return CreateTestWindowWithDelegate(NULL, id, gfx::Rect(), parent);
}

Window* CreateTestWindowWithBounds(const gfx::Rect& bounds, Window* parent) {
  return CreateTestWindowWithDelegate(NULL, 0, bounds, parent);
}

Window* CreateTestWindow(SkColor color,
                         int id,
                         const gfx::Rect& bounds,
                         Window* parent) {
  return CreateTestWindowWithDelegate(new ColorTestWindowDelegate(color),
                                      id, bounds, parent);
}

Window* CreateTestWindowWithDelegateAndType(WindowDelegate* delegate,
                                            ui::wm::WindowType type,
                                            int id,
                                            const gfx::Rect& bounds,
                                            Window* parent) {
  Window* window = new Window(delegate);
  window->set_id(id);
  window->SetType(type);
  window->SetBounds(bounds);
  window->Show();
  if (parent)
    parent->AddChild(window);
  window->SetProperty(kCanMaximizeKey, true);
  return window;
}


Window* CreateTestWindowWithDelegate(WindowDelegate* delegate,
                                     int id,
                                     const gfx::Rect& bounds,
                                     Window* parent) {
  return CreateTestWindowWithDelegateAndType(
      delegate, ui::wm::WINDOW_TYPE_NORMAL, id, bounds, parent);
}

}  // nnamespace win
}  // namespace azer
