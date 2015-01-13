#include "azer/ui/win/window_event_dispatcher.h"

#include "azer/ui/win/window.h"
#include "azer/ui/win/window_tree_host.h"

namespace azer {
namespace win {

WindowEventDispatcher::WindowEventDispatcher(WindowTreeHost* host)
    : host_(host) {
}

WindowEventDispatcher::~WindowEventDispatcher() {
}

////////////////////////////////////////////////////////////////////////////////
// WindowEventDispatcher, ui::EventProcessor implementation:
ui::EventTarget* WindowEventDispatcher::GetRootTarget() {
  return window();
}

void WindowEventDispatcher::OnEventProcessingStarted(ui::Event* event) {
}

Window* WindowEventDispatcher::window() {
  return host_->window();
}

const Window* WindowEventDispatcher::window() const {
  return host_->window();
}

bool WindowEventDispatcher::CanDispatchToTarget(ui::EventTarget* target) {
  return true;
}

}  // namespace win
}  // namespace azer
