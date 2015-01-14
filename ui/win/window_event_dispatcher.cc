#include "azer/ui/win/window_event_dispatcher.h"

#include "azer/ui/win/window.h"
#include "azer/ui/win/window_tree_host.h"

namespace azer {
namespace win {

typedef ui::EventDispatchDetails DispatchDetails;

WindowEventDispatcher::WindowEventDispatcher(WindowTreeHost* host)
    : host_(host) 
    , event_dispatch_target_(NULL)
    , old_dispatch_target_(NULL) {
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
  return event_dispatch_target_ == target;
}

::ui::EventDispatchDetails WindowEventDispatcher::PreDispatchEvent(
    ::ui::EventTarget* target, ::ui::Event* event) {
  Window* target_window = static_cast<Window*>(target);
  CHECK(window()->Contains(target_window));

  if (event->IsMouseEvent()) {
    PreDispatchMouseEvent(target_window, static_cast<ui::MouseEvent*>(event));
  } else {
    LOG(ERROR) << "Unsupport event.";
  }
  old_dispatch_target_ = event_dispatch_target_;
  event_dispatch_target_ = static_cast<Window*>(target);
  return DispatchDetails();
}
::ui::EventDispatchDetails WindowEventDispatcher::PostDispatchEvent(
    ::ui::EventTarget* target, const ::ui::Event& event) {
  DispatchDetails details;
  if (!target || target != event_dispatch_target_)
    details.target_destroyed = true;
  event_dispatch_target_ = old_dispatch_target_;
  old_dispatch_target_ = NULL;
#ifndef NDEBUG
  DCHECK(!event_dispatch_target_ || window()->Contains(event_dispatch_target_));
#endif

  return details;
}

void WindowEventDispatcher::PreDispatchLocatedEvent(Window* target,
                                                    ui::LocatedEvent* event) {
}

void WindowEventDispatcher::PreDispatchMouseEvent(Window* target,
                                                  ui::MouseEvent* event) {
  PreDispatchLocatedEvent(target, event);
}
}  // namespace win
}  // namespace azer
