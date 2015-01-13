#include "azer/ui/win/window_event_dispatcher.h"

namespace azer {
namespace win {
WindowEventDispatcher::WindowEventDispatcher(WindowTreeHost* host)
    : host_(host) {
}

WindowEventDispatcher::WindowEventDispatcher() {
}

////////////////////////////////////////////////////////////////////////////////
// WindowEventDispatcher, ui::EventProcessor implementation:
ui::EventTarget* WindowEventDispatcher::GetRootTarget() {
  return window();
}

void OnEventProcessingStarted(ui::Event* event) {
}

Window* WindowEventDispatcher::window() {
  return host_->window();
}

const Window* WindowEventDispatcher::window() const {
  return host_->window();
}

}  // namespace win
}  // namespace azer
