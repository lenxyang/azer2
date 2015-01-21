#pragma once

#include "base/message_loop/message_loop.h"
#include "ui/events/event_constants.h"
#include "ui/events/event_processor.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "azer/base/export.h"

#include "azer/ui/widget/client/capture_delegate.h"

namespace azer {
namespace widget {

class Widget;
class WidgetTreeHost;

class AZER_EXPORT WidgetEventDispatcher : public ::ui::EventProcessor
                                        , public client::CaptureDelegate {
 public:
  WidgetEventDispatcher(WidgetTreeHost* host);
  ~WidgetEventDispatcher() override;

  const gfx::Point& last_mouse_location() const { return last_mouse_location_;}

  Widget* mouse_pressed_handler() { return mouse_pressed_handler_;}
  Widget* mouse_moved_handler() { return mouse_moved_handler_;}

  void OnPostNotifiedWidgetDestroying(Widget* widget);
 protected:
  // override from client::CaptureDelegate
  void UpdateCapture(Widget* old_capture, Widget* new_capture) override;
  void OnOtherRootGotCapture() override;
  void SetNativeCapture() override;
  void ReleaseNativeCapture() override;

  // The parameter for OnWidgetHidden() to specify why window is hidden.
  enum WidgetHiddenReason {
    WINDOW_DESTROYED,  // Widget is destroyed.
    WINDOW_HIDDEN,     // Widget is hidden.
    WINDOW_MOVING,     // Widget is temporarily marked as hidden due to move
                       // across root windows.
  };
  void OnWidgetHidden(Widget* invisible, WidgetHiddenReason reason);

  // Creates and dispatches synthesized mouse move event using the current mouse
  // location.
  ui::EventDispatchDetails SynthesizeMouseMoveEvent() WARN_UNUSED_RESULT;
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
  ui::EventDispatchDetails DispatchMouseEnterOrExit(const ui::MouseEvent& event,
                                                    ui::EventType type);

  void SetLastMouseLocation(Widget* widget, const gfx::Point& location);

  Widget* root();

  WidgetTreeHost* host_;

  Widget* mouse_pressed_handler_;
  Widget* mouse_moved_handler_;
  Widget* old_dispatch_target_;
  Widget* event_dispatch_target_;
  

  bool synthesize_mouse_move_;
  gfx::Point last_mouse_location_;
  DISALLOW_COPY_AND_ASSIGN(WidgetEventDispatcher);
};
}  // namespace widget
}  // namespace azer
