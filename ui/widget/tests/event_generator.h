#pragma once

#include <list>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "base/time/default_tick_clock.h"
#include "ui/events/event_constants.h"
#include "ui/events/event.h"
#include "ui/events/event_source.h"
#include "ui/events/event_target.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/geometry/point.h"

namespace azer {
namespace widget {

class Widget;

namespace testing {

typedef base::Callback<void(ui::EventType, const gfx::Vector2dF&)>
        ScrollStepCallback;

class EventGenerator;

// A delegate interface for EventGenerator to abstract platform-specific event
// targeting and coordinate conversion.
class EventGeneratorDelegate {
 public:
  virtual ~EventGeneratorDelegate() {}

  // Set the context of the delegate, whilst it is being used by an active
  // EventGenerator.
  virtual void SetContext(EventGenerator* owner, Widget* root_window,
                            Widget* window) {}

  // The ui::EventTarget at the given |location|.
  virtual ui::EventTarget* GetTargetAt(const gfx::Point& location) = 0;

  // The ui::EventSource for the given |target|.
  virtual ui::EventSource* GetEventSource(ui::EventTarget* target) = 0;

  // Helper functions to determine the center point of |target| or |window|.
  virtual gfx::Point CenterOfTarget(const ui::EventTarget* target) const = 0;
  virtual gfx::Point CenterOfWindow(Widget* window) const = 0;

  // Convert a point between API's coordinates and |target|'s coordinates.
  virtual void ConvertPointFromTarget(const ui::EventTarget* target,
                                      gfx::Point* point) const = 0;
  virtual void ConvertPointToTarget(const ui::EventTarget* target,
                                    gfx::Point* point) const = 0;

  // Convert a point from the coordinate system in the host that contains
  // |hosted_target| into the root window's coordinate system.
  virtual void ConvertPointFromHost(const ui::EventTarget* hosted_target,
                                    gfx::Point* point) const = 0;
};


class EventGenerator {
 public:
  EventGenerator(Widget* host, const gfx::Point& initial_location);
  ~EventGenerator();

  // Generates a left button press event.
  void PressLeftButton();

  // Generates a left button release event.
  void ReleaseLeftButton();

  // Generates events to click (press, release) left button.
  void ClickLeftButton();

  // Generates a double click event using the left button.
  void DoubleClickLeftButton();

  // Generates a right button press event.
  void PressRightButton();

  // Generates a right button release event.
  void ReleaseRightButton();

  // Moves the mouse wheel by |delta_x|, |delta_y|.
  void MoveMouseWheel(int delta_x, int delta_y);

  // Generates events to move mouse to be the given |point| in the
  // |current_root_window_|'s host window coordinates.
  void MoveMouseToInHost(const gfx::Point& point_in_host);
  void MoveMouseToInHost(int x, int y) {
    MoveMouseToInHost(gfx::Point(x, y));
  }

  // Generates events to move mouse to be the given |point| in screen
  // coordinates.
  void MoveMouseTo(const gfx::Point& point_in_screen, int count);
  void MoveMouseTo(const gfx::Point& point_in_screen) {
    MoveMouseTo(point_in_screen, 1);
  }
  void MoveMouseTo(int x, int y) {
    MoveMouseTo(gfx::Point(x, y));
  }

  // Generates events to move mouse to be the given |point| in |window|'s
  // coordinates.
  void MoveMouseRelativeTo(const ui::EventTarget* window, const gfx::Point& point);
  void MoveMouseRelativeTo(const ui::EventTarget* window, int x, int y) {
    MoveMouseRelativeTo(window, gfx::Point(x, y));
  }

  void MoveMouseBy(int x, int y) {
    MoveMouseTo(current_location_ + gfx::Vector2d(x, y));
  }

  // Generates events to drag mouse to given |point|.
  void DragMouseTo(const gfx::Point& point);
  
  void DragMouseTo(int x, int y) {
    DragMouseTo(gfx::Point(x, y));
  }

  void DragMouseBy(int dx, int dy) {
    DragMouseTo(current_location_ + gfx::Vector2d(dx, dy));
  }

  // Generates a key press event. On platforms except Windows and X11, a key
  // event without native_event() is generated. Note that ui::EF_ flags should
  // be passed as |flags|, not the native ones like 'ShiftMask' in <X11/X.h>.
  // TODO(yusukes): Support native_event() on all platforms.
  void PressKey(ui::KeyboardCode key_code, int flags);

  // Generates a key release event. On platforms except Windows and X11, a key
  // event without native_event() is generated. Note that ui::EF_ flags should
  // be passed as |flags|, not the native ones like 'ShiftMask' in <X11/X.h>.
  // TODO(yusukes): Support native_event() on all platforms.
  void ReleaseKey(ui::KeyboardCode key_code, int flags);

  // Dispatch the event to the WindowEventDispatcher.
  void Dispatch(ui::Event* event);

  static EventGeneratorDelegate* default_delegate;
 protected:
  void Init(Widget* root_window, Widget* window_context);
  // Dispatch a key event to the WindowEventDispatcher.
  void DispatchKeyEvent(bool is_press, ui::KeyboardCode key_code, int flags);

  void UpdateCurrentDispatcher(const gfx::Point& point);
  void PressButton(int flag);
  void ReleaseButton(int flag);

  gfx::Point GetLocationInCurrentRoot() const;
  gfx::Point CenterOfWindow(const ui::EventTarget* window) const;

  void DispatchNextPendingEvent();
  void DoDispatchEvent(ui::Event* event, bool async);

  const EventGeneratorDelegate* delegate() const;
  EventGeneratorDelegate* delegate();


  scoped_ptr<EventGeneratorDelegate> delegate_;
  
  gfx::Point current_location_;
  ui::EventTarget* current_target_;

  int flags_;
  bool grab_;
  std::list<ui::Event*> pending_events_;
  bool async_;
  scoped_ptr<base::TickClock> tick_clock_;

  DISALLOW_COPY_AND_ASSIGN(EventGenerator);
};
}  // namespace testing
}  // namespace widget
}  // namespace azer
