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
#include "azer/ui/widget/client/capture_client.h"

typedef ui::EventDispatchDetails DispatchDetails;

namespace azer {
namespace widget {
WidgetEventDispatcher::WidgetEventDispatcher(WidgetTreeHost* host)
    : host_(host)
    , mouse_pressed_handler_(NULL)
    , mouse_moved_handler_(NULL)
    , old_dispatch_target_(NULL)
    , event_dispatch_target_(NULL)
    , synthesize_mouse_move_(false)
    , observer_manager_(this)
    , held_event_factory_(this) {
}

WidgetEventDispatcher::~WidgetEventDispatcher() {
}

ui::EventTarget* WidgetEventDispatcher::GetRootTarget() {
  return host_->widget();
}

void WidgetEventDispatcher::OnEventProcessingStarted(ui::Event* event) {
}

bool WidgetEventDispatcher::CanDispatchToTarget(ui::EventTarget* target) {
  return event_dispatch_target_ == target;
}


ui::EventDispatchDetails WidgetEventDispatcher::PreDispatchEvent(
    ui::EventTarget* target, ui::Event* event) {
  Widget* target_widget = static_cast<Widget*>(target);
  CHECK(root()->Contains(target_widget));
  if (event->IsMouseEvent()) {
    PreDispatchMouseEvent(target_widget, static_cast<ui::MouseEvent*>(event));
  }

  old_dispatch_target_ = event_dispatch_target_;
  event_dispatch_target_ = target_widget;
  return DispatchDetails();
}

ui::EventDispatchDetails WidgetEventDispatcher::PostDispatchEvent(
    ui::EventTarget* target, const ui::Event& event) {
  event_dispatch_target_ = old_dispatch_target_;
  old_dispatch_target_ = NULL;
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
  // system), or a synthetic event created in root-widget (in which case, the
  // event's target will be NULL, and the event will be in the root-widget's
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
  // |mouse_moved_handler_| may have been set to a Widget in a different root
  // (see below). Clear it here to ensure we don't end up referencing a stale
  // Widget.
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
    // Make all subsequent mouse events go to the capture widget. We shouldn't
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
  // Widgets provides the TrackMouseEvents API which allows us to rely on the
  // OS to send us the mouse exit events (WM_MOUSELEAVE). Additionally on
  // desktop Widgets, every top level widget could potentially have its own
  // root widget, in which case this function will get called whenever those
  // widgets grab mouse capture. Sending mouse exit messages in these cases
  // causes subtle bugs like (crbug.com/394672).
#if !defined(OS_WIN)
  if (mouse_moved_handler_) {
    // Dispatch a mouse exit to reset any state associated with hover. This is
    // important when going from no widget having capture to a widget having
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

void WidgetEventDispatcher::OnPostNotifiedWidgetDestroying(Widget* widget) {
  OnWidgetHidden(widget, WIDGET_DESTROYED);
}

void WidgetEventDispatcher::OnWidgetHidden(Widget* invisible,
                                           WidgetHiddenReason reason) {
  // If the widget the mouse was pressed in becomes invisible, it should no
  // longer receive mouse events.
  if (invisible->Contains(mouse_pressed_handler_))
    mouse_pressed_handler_ = NULL;
  if (invisible->Contains(mouse_moved_handler_))
    mouse_moved_handler_ = NULL;

  // If events are being dispatched from a nested message-loop, and the target
  // of the outer loop is hidden or moved to another dispatcher during
  // dispatching events in the inner loop, then reset the target for the outer
  // loop.
  if (invisible->Contains(old_dispatch_target_))
    old_dispatch_target_ = NULL;

  // Do not clear the capture, and the |event_dispatch_target_| if the
  // widget is moving across hosts, because the target itself is actually still
  // visible and clearing them stops further event processing, which can cause
  // unexpected behaviors. See crbug.com/157583
  if (reason != WIDGET_MOVING) {
    // We don't ask |invisible| here, because invisible may have been removed
    // from the widget hierarchy already by the time this function is called
    // (OnWidgetDestroyed).
    client::CaptureClient* capture_client =
        client::GetCaptureClient(host_->widget());
    Widget* capture_widget =
        capture_client ? capture_client->GetCaptureWidget() : NULL;

    if (invisible->Contains(event_dispatch_target_))
      event_dispatch_target_ = NULL;

    // If the ancestor of the capture widget is hidden, release the capture.
    // Note that this may delete the widget so do not use capture_widget
    // after this.
    if (invisible->Contains(capture_widget) && invisible != root())
      capture_widget->ReleaseCapture();
  }
}

////////////////////////////////////////////////////////////////////////////////
// WidgetEventDispatcher, WidgetObserver implementation:

void WidgetEventDispatcher::OnWidgetDestroying(Widget* widget) {
  if (!host_->widget()->Contains(widget))
    return;

  DispatchMouseExitToHidingWidget(widget);
  SynthesizeMouseMoveAfterChangeToWidget(widget);
}

void WidgetEventDispatcher::DispatchMouseExitToHidingWidget(Widget* widget) {
}

void WidgetEventDispatcher::OnWidgetDestroyed(Widget* widget) {
  // We observe all widgets regardless of what root Widget (if any) they're
  // attached to.
  observer_manager_.Remove(widget);
}

void WidgetEventDispatcher::OnWidgetAddedToRootWidget(Widget* attached) {
  if (!observer_manager_.IsObserving(attached))
    observer_manager_.Add(attached);

  if (!host_->widget()->Contains(attached))
    return;

  SynthesizeMouseMoveAfterChangeToWidget(attached);
}

void WidgetEventDispatcher::OnWidgetRemovingFromRootWidget(Widget* detached) {
  if (!host_->widget()->Contains(detached))
    return;

  DCHECK(client::GetCaptureWidget(root()) != root());

  DispatchMouseExitToHidingWidget(detached);
  SynthesizeMouseMoveAfterChangeToWidget(detached);

  // Hiding the widget releases capture which can implicitly destroy the widget
  // so the widget may no longer be valid after this call.
  OnWidgetHidden(detached, WIDGET_HIDDEN);
}

void WidgetEventDispatcher::OnWidgetVisibilityChanging(Widget* widget,
                                                       bool visible) {
  if (!host_->widget()->Contains(widget))
    return;

  DispatchMouseExitToHidingWidget(widget);
}

void WidgetEventDispatcher::OnWidgetVisibilityChanged(Widget* widget,
                                                      bool visible) {
  if (!host_->widget()->Contains(widget))
    return;

  if (widget->ContainsPointInRoot(GetLastMouseLocationInRoot()))
    PostSynthesizeMouseMove();

  // Hiding the widget releases capture which can implicitly destroy the widget
  // so the widget may no longer be valid after this call.
  if (!visible)
    OnWidgetHidden(widget, WIDGET_HIDDEN);
}

ui::EventDispatchDetails WidgetEventDispatcher::DispatchHeldEvents() {
  return DispatchDetails();
}

void WidgetEventDispatcher::OnWidgetBoundsChanged(Widget* widget,
                                                  const gfx::Rect& old_bounds,
                                                  const gfx::Rect& new_bounds) {
  if (!host_->widget()->Contains(widget))
    return;

  if (widget == host_->widget()) {
    TRACE_EVENT1("ui", "WidgetEventDispatcher::OnWidgetBoundsChanged(root)",
                 "size", new_bounds.size().ToString());

    DispatchDetails details = DispatchHeldEvents();
    if (details.dispatcher_destroyed)
      return;

    synthesize_mouse_move_ = false;
  }

  if (widget->IsVisible() && !widget->ignore_events()) {
    gfx::Rect old_bounds_in_root = old_bounds, new_bounds_in_root = new_bounds;
    Widget::ConvertRectToTarget(widget->parent(), host_->widget(),
                                &old_bounds_in_root);
    Widget::ConvertRectToTarget(widget->parent(), host_->widget(),
                                &new_bounds_in_root);
    gfx::Point last_mouse_location = GetLastMouseLocationInRoot();
    if (old_bounds_in_root.Contains(last_mouse_location) !=
        new_bounds_in_root.Contains(last_mouse_location)) {
      PostSynthesizeMouseMove();
    }
  }
}

void WidgetEventDispatcher::PostSynthesizeMouseMove() {
  if (synthesize_mouse_move_)
    return;
  synthesize_mouse_move_ = true;
  base::MessageLoop::current()->PostNonNestableTask(
      FROM_HERE,
      base::Bind(base::IgnoreResult(
          &WidgetEventDispatcher::SynthesizeMouseMoveEvent),
                 held_event_factory_.GetWeakPtr()));
}


void WidgetEventDispatcher::SynthesizeMouseMoveAfterChangeToWidget(
    Widget* widget) {
  if (widget->IsVisible() &&
      widget->ContainsPointInRoot(GetLastMouseLocationInRoot())) {
    PostSynthesizeMouseMove();
  }
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

  gfx::Point root_mouse_location = GetLastMouseLocationInRoot();
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
}  // namespace widget
}  // namespace azer
