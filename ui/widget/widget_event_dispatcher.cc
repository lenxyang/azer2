#include "azer/ui/widget/widget_event_dispatcher.h"

#include "base/bind.h"
#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "ui/compositor/dip_util.h"
#include "ui/events/event.h"

#include "azer/ui/widget/widget_tree_host.h"
#include "azer/ui/widget/widget.h"

typedef ui::EventDispatchDetails DispatchDetails;

namespace azer {
namespace widget {
WidgetEventDispatcher::WidgetEventDispatcher(WidgetTreeHost* host)
    : host_(host)
    , mouse_pressed_handler_(NULL)
    , mouse_moved_handler_(NULL) {
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
  return host_->root();
}
}  // namespace widget
}  // namespace azer
