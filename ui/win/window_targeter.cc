#include "azer/ui/win/window_targeter.h"



#include "azer/ui/win/window.h"
#include "azer/ui/win/window_delegate.h"
#include "azer/ui/win/window_tree_host.h"

namespace azer {
namespace win {

namespace {
bool IsLocatedEvent(const ui::Event& event) {
  return event.IsMouseEvent() || event.IsTouchEvent() ||
      event.IsScrollEvent() || event.IsGestureEvent();
}
}  // namespace

WindowTargeter::WindowTargeter() {
}

WindowTargeter::~WindowTargeter() {
}

::ui::EventTarget* WindowTargeter::FindTargetForEvent(::ui::EventTarget* root,
                                                      ::ui::Event* event) {
  Window* window = static_cast<Window*>(root);
  Window* target = event->IsKeyEvent() ?
      FindTargetForKeyEvent(window, *static_cast<ui::KeyEvent*>(event)) :
      static_cast<Window*>(EventTargeter::FindTargetForEvent(root, event));
  return target;
}
}  // namespace win
}  // namespace azer
