#include "azer/ui/widget/widget_event_dispatcher.h"

#include "base/bind.h"
#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "ui/compositor/dip_util.h"
#include "ui/events/event.h"

#include "azer/ui/widget/widget_tree_host.h"
#include "azer/ui/widget/widget_delegate.h"
#include "azer/ui/widget/widget.h"

typedef ui::EventDispatchDetails DispatchDetails;

namespace azer {
namespace widget {
WidgetEventDispatcher::WidgetEventDispatcher(WidgetTreeHost* host)
    : host_(host)
    , mouse_pressed_handler_(NULL)
    , mouse_moved_handler_(NULL) 
    , synthesize_mouse_move_(false) {
}

WidgetEventDispatcher::~WidgetEventDispatcher() {
}

ui::EventTarget* WidgetEventDispatcher::GetRootTarget() {
  return host_->widget();
}

void WidgetEventDispatcher::OnEventProcessingStarted(ui::Event* event) {
}

bool WidgetEventDispatcher::CanDispatchToTarget(ui::EventTarget* target) {
  return true;
}

ui::EventDispatchDetails WidgetEventDispatcher::SynthesizeMouseMoveEvent() {
  DispatchDetails details;
  if (!synthesize_mouse_move_)
    return details;
  synthesize_mouse_move_ = false;

  // If one of the mouse buttons is currently down, then do not synthesize a
  // mouse-move event. In such cases, aura could synthesize a DRAGGED event
  // instead of a MOVED event, but in multi-display/multi-host scenarios, the
  // DRAGGED event can be synthesized in the incorrect host. So avoid
  // synthesizing any events at all.

  gfx::Point root_mouse_location = last_mouse_location();
  if (!root()->bounds().Contains(root_mouse_location))
    return details;
  gfx::Point host_mouse_location = root_mouse_location;
  host_->ConvertPointToHost(&host_mouse_location);
  ui::MouseEvent event(ui::ET_MOUSE_MOVED,
                       host_mouse_location,
                       host_mouse_location,
                       ui::EF_IS_SYNTHESIZED,
                       0);
  return OnEventFromSource(&event);
}


ui::EventDispatchDetails WidgetEventDispatcher::PreDispatchEvent(
    ui::EventTarget* target, ui::Event* event) {
  Widget* target_widget = static_cast<Widget*>(target);
  CHECK(root()->Contains(target_widget));
  if (event->IsMouseEvent()) {
    PreDispatchMouseEvent(target_widget, static_cast<ui::MouseEvent*>(event));
  }

  return DispatchDetails();
}

ui::EventDispatchDetails WidgetEventDispatcher::PostDispatchEvent(
    ui::EventTarget* target, const ui::Event& event) {
  return DispatchDetails();
}

ui::EventDispatchDetails WidgetEventDispatcher::DispatchMouseEnterOrExit(
    const ui::MouseEvent& event,
    ui::EventType type) {
  if (event.type() != ui::ET_MOUSE_CAPTURE_CHANGED &&
      !(event.flags() & ui::EF_IS_SYNTHESIZED)) {
    SetLastMouseLocation(root(), event.root_location());
  }

  if (!mouse_moved_handler_
      || !mouse_moved_handler_->delegate()
      || !root()->Contains(mouse_moved_handler_)) {
    return DispatchDetails();
  }

  // |event| may be an event in the process of being dispatched to a target (in
  // which case its locations will be in the event's target's coordinate
  // system), or a synthetic event created in root-window (in which case, the
  // event's target will be NULL, and the event will be in the root-window's
  // coordinate system.
  Widget* target = static_cast<Widget*>(event.target());
  if (!target)
    target = root();
  ui::MouseEvent translated_event(event,
                                  target,
                                  mouse_moved_handler_,
                                  type,
                                  event.flags() | ui::EF_IS_SYNTHESIZED);
  return DispatchEvent(mouse_moved_handler_, &translated_event);
}

void WidgetEventDispatcher::PreDispatchMouseEvent(Widget* target, 
                                                  ui::MouseEvent* event) {
  SetLastMouseLocation(target, event->root_location());
  switch (event->type()) {
    case ui::ET_MOUSE_EXITED:
      mouse_moved_handler_ = NULL;
      break;
    case ui::ET_MOUSE_MOVED:
      // Send an exit to the current |mouse_moved_handler_| and an enter to
      // |target|. Take care that both us and |target| aren't destroyed during
      // dispatch
      if (target != mouse_moved_handler_) {
        DispatchDetails details = 
            DispatchMouseEnterOrExit(*event, ui::ET_MOUSE_EXITED);
        if (details.dispatcher_destroyed) {
          event->SetHandled();
          return;
        }

        mouse_moved_handler_ = target;
        details = DispatchMouseEnterOrExit(*event, ui::ET_MOUSE_ENTERED);
        if (details.dispatcher_destroyed || details.target_destroyed) {
          event->SetHandled();
          return;
        }
      }
      break;
    default:
      break;
  }
}

Widget* WidgetEventDispatcher::root() {
  return host_->widget();
}

// override from client::CaptureDelegate
void WidgetEventDispatcher::UpdateCapture(Widget* old_capture, 
                                          Widget* new_capture) {
  // |mouse_moved_handler_| may have been set to a Window in a different root
  // (see below). Clear it here to ensure we don't end up referencing a stale
  // Window.
  if (mouse_moved_handler_ && !root()->Contains(mouse_moved_handler_))
    mouse_moved_handler_ = NULL;

  if (old_capture && old_capture->GetRootWidget() == root() &&
      old_capture->delegate()) {
    // Send a capture changed event with bogus location data.
    ui::MouseEvent event(ui::ET_MOUSE_CAPTURE_CHANGED, gfx::Point(),
                         gfx::Point(), 0, 0);

    DispatchDetails details = DispatchEvent(old_capture, &event);
    if (details.dispatcher_destroyed)
      return;

    if (!details.target_destroyed)
      old_capture->delegate()->OnCaptureLost();
  }

  if (new_capture) {
    // Make all subsequent mouse events go to the capture window. We shouldn't
    // need to send an event here as OnCaptureLost() should take care of that.
    if (mouse_moved_handler_) {
      mouse_moved_handler_ = new_capture;
    }
  } else {
    // Make sure mouse_moved_handler gets updated.
    DispatchDetails details = SynthesizeMouseMoveEvent();
    if (details.dispatcher_destroyed)
      return;
  }
  mouse_pressed_handler_ = NULL;
}

void WidgetEventDispatcher::OnOtherRootGotCapture() {
  // Windows provides the TrackMouseEvents API which allows us to rely on the
  // OS to send us the mouse exit events (WM_MOUSELEAVE). Additionally on
  // desktop Windows, every top level window could potentially have its own
  // root window, in which case this function will get called whenever those
  // windows grab mouse capture. Sending mouse exit messages in these cases
  // causes subtle bugs like (crbug.com/394672).
#if !defined(OS_WIN)
  if (mouse_moved_handler_) {
    // Dispatch a mouse exit to reset any state associated with hover. This is
    // important when going from no window having capture to a window having
    // capture because we do not dispatch ET_MOUSE_CAPTURE_CHANGED in this case.
    DispatchDetails details = DispatchMouseExitAtPoint(
        GetLastMouseLocationInRoot());
    if (details.dispatcher_destroyed)
      return;
  }
#endif

  mouse_moved_handler_ = NULL;
  mouse_pressed_handler_ = NULL;
}

void WidgetEventDispatcher::SetNativeCapture() {
  host_->SetCapture();
}

void WidgetEventDispatcher::ReleaseNativeCapture() {
  host_->ReleaseCapture();
}

void WidgetEventDispatcher::SetLastMouseLocation(Widget* widget, 
                                                 const gfx::Point& location) {
  last_mouse_location_ = location;
}
}  // namespace widget
}  // namespace azer
