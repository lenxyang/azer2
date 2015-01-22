#pragma once

#include "base/message_loop/message_loop.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/scoped_observer.h"
#include "ui/events/event_constants.h"
#include "ui/events/event_processor.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "azer/base/export.h"

#include "azer/ui/widget/client/capture_delegate.h"
#include "azer/ui/widget/widget_observer.h"

namespace azer {
namespace widget {

class Widget;
class WidgetTreeHost;

class AZER_EXPORT WidgetEventDispatcher : public ::ui::EventProcessor
                                        , public WidgetObserver
                                        , public client::CaptureDelegate {
 public:
  WidgetEventDispatcher(WidgetTreeHost* host);
  ~WidgetEventDispatcher() override;

  const gfx::Point& GetLastMouseLocationInRoot() const {return last_mouse_location_;}
  const gfx::Point& last_mouse_location() const { return last_mouse_location_;}

  Widget* mouse_pressed_handler() { return mouse_pressed_handler_;}
  Widget* mouse_moved_handler() { return mouse_moved_handler_;}

  void OnPostNotifiedWidgetDestroying(Widget* widget);

  // Overridden from WindowObserver:
  void OnWidgetDestroying(Widget* widget) override;
  void OnWidgetDestroyed(Widget* widget) override;
  void OnWidgetAddedToRootWidget(Widget* widget) override;
  void OnWidgetRemovingFromRootWidget(Widget* widget) override;
  void OnWidgetVisibilityChanging(Widget* widget, bool visible) override;
  void OnWidgetVisibilityChanged(Widget* widget, bool visible) override;
  void OnWidgetBoundsChanged(Widget* widget,
                             const gfx::Rect& old_bounds,
                             const gfx::Rect& new_bounds) override;

  // We hold and aggregate mouse drags and touch moves as a way of throttling
  // resizes when HoldMouseMoves() is called. The following methods are used to
  // dispatch held and newly incoming mouse and touch events, typically when an
  // event other than one of these needs dispatching or a matching
  // ReleaseMouseMoves()/ReleaseTouchMoves() is called.  NOTE: because these
  // methods dispatch events from WindowTreeHost the coordinates are in terms of
  // the root.
  ui::EventDispatchDetails DispatchHeldEvents() WARN_UNUSED_RESULT;
 protected:
  // override from client::CaptureDelegate
  void UpdateCapture(Widget* old_capture, Widget* new_capture) override;
  void OnOtherRootGotCapture() override;
  void SetNativeCapture() override;
  void ReleaseNativeCapture() override;

  // The parameter for OnWidgetHidden() to specify why widget is hidden.
  enum WidgetHiddenReason {
    WIDGET_DESTROYED,  // Widget is destroyed.
    WIDGET_HIDDEN,     // Widget is hidden.
    WIDGET_MOVING,     // Widget is temporarily marked as hidden due to move
                       // across root widgets.
  };
  void OnWidgetHidden(Widget* invisible, WidgetHiddenReason reason);
 protected:
  // Overridden from ui::EventProcessor:
  ui::EventTarget* GetRootTarget() override;
  void OnEventProcessingStarted(ui::Event* event) override;

  // Overridden from ui::EventDispatcherDelegate.
  bool CanDispatchToTarget(ui::EventTarget* target) override;
  ui::EventDispatchDetails PreDispatchEvent(ui::EventTarget* target,
                                            ui::Event* event) override;
  ui::EventDispatchDetails PostDispatchEvent(ui::EventTarget* target,
                                             const ui::Event& event) override;

  void PreDispatchMouseEvent(Widget* target, ui::MouseEvent* event);

  // Dispatches OnMouseExited to the |widget| which is hiding if necessary.
  void DispatchMouseExitToHidingWidget(Widget* widget);

  // Dispatches the specified event type (intended for enter/exit) to the
  // |mouse_moved_handler_|.
  ui::EventDispatchDetails DispatchMouseEnterOrExit(const ui::MouseEvent& event,
                                                    ui::EventType type);

  void SetLastMouseLocation(Widget* widget, const gfx::Point& location);

  void PostSynthesizeMouseMove();
  void SynthesizeMouseMoveAfterChangeToWidget(Widget* widget);
  ui::EventDispatchDetails SynthesizeMouseMoveEvent() WARN_UNUSED_RESULT;

  Widget* root();
  WidgetTreeHost* host_;

  Widget* mouse_pressed_handler_;
  Widget* mouse_moved_handler_;
  Widget* old_dispatch_target_;
  Widget* event_dispatch_target_;

  bool synthesize_mouse_move_;
  gfx::Point last_mouse_location_;

  ScopedObserver<Widget, WidgetObserver> observer_manager_;

  // Used to schedule DispatchHeldEvents() when |move_hold_count_| goes to 0.
  base::WeakPtrFactory<WidgetEventDispatcher> held_event_factory_;
  DISALLOW_COPY_AND_ASSIGN(WidgetEventDispatcher);
};
}  // namespace widget
}  // namespace azer
