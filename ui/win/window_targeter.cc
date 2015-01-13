#include "azer/ui/win/window_targeter.h"

#include "azer/ui/win/window.h"
#include "azer/ui/win/window_delegate.h"
#include "azer/ui/win/window_tree_host.h"
#include "azer/ui/win/client/focus_client.h"
#include "azer/ui/win/client/event_client.h"

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

Window* WindowTargeter::FindTargetForKeyEvent(Window* window,
                                              const ui::KeyEvent& key) {
  Window* root_window = window->GetRootWindow();
  FocusClient* focus_client = GetFocusClient(root_window);
  Window* focused_window = focus_client->GetFocusedWindow();
  if (!focused_window)
    return window;

  EventClient* event_client = GetEventClient(root_window);
  if (event_client &&
      !event_client->CanProcessEventsWithinSubtree(focused_window)) {
    focus_client->FocusWindow(NULL);
    return NULL;
  }
  return focused_window ? focused_window : window;
}
}  // namespace win
}  // namespace azer
