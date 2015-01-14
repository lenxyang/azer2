#include "azer/ui/win/tests/test_window.h"

#include "base/strings/string_number_conversions.h"
#include "azer/ui/win/constants.h"
#include "azer/ui/win/window.h"
#include "ui/gfx/rect.h"


namespace azer {
namespace win {

Window* CreateTestWindowWithDelegateAndType(WindowDelegate* delegate,
                                            ui::wm::WindowType type,
                                            int id,
                                            const gfx::Rect& bounds,
                                            Window* parent) {
  Window* window = new Window(delegate);
  window->set_id(id);
  window->SetType(type);
  window->Init(aura::WINDOW_LAYER_TEXTURED);
  window->SetBounds(bounds);
  window->Show();
  if (parent)
    parent->AddChild(window);
  window->SetProperty(aura::client::kCanMaximizeKey, true);
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
