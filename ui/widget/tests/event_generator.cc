#include "azer/ui/widget/tests/event_generator.h"

#include "base/bind.h"
#include "base/location.h"
#include "base/memory/scoped_ptr.h"
#include "base/single_thread_task_runner.h"
#include "base/thread_task_runner_handle.h"
#include "ui/events/event_source.h"
#include "ui/events/event_utils.h"
#include "ui/events/test/events_test_utils.h"
#include "ui/gfx/geometry/vector2d_conversions.h"
#include "azer/ui/widget/widget.h"
#include "azer/ui/widget/widget_tree_host.h"

#if defined(USE_X11)
#include <X11/Xlib.h>
#include "ui/events/test/events_test_utils_x11.h"
#endif

#if defined(OS_WIN)
#include "ui/events/keycodes/keyboard_code_conversion.h"
#endif

#include "azer/ui/widget/widget.h"


namespace azer {
namespace widget {
namespace testing {
namespace {

class TestKeyEvent : public ui::KeyEvent {
 public:
  TestKeyEvent(const base::NativeEvent& native_event, int flags)
      : KeyEvent(native_event) {
    set_flags(flags);
  }
};

class TestTouchEvent : public ui::TouchEvent {
 public:
  TestTouchEvent(ui::EventType type,
                 const gfx::Point& root_location,
                 int touch_id,
                 int flags,
                 base::TimeDelta timestamp)
      : TouchEvent(type, root_location, flags, touch_id, timestamp,
                   1.0f, 1.0f, 0.0f, 0.0f) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TestTouchEvent);
};

const int kAllButtonMask = ui::EF_LEFT_MOUSE_BUTTON | ui::EF_RIGHT_MOUSE_BUTTON;

}  // namespace

EventGeneratorDelegate* EventGenerator::default_delegate = NULL;

EventGenerator::EventGenerator(Widget* root_window, 
                               const gfx::Point& initial_location)
    : current_target_(NULL)
    , flags_(0)
    , grab_(false)
    , async_(false)
    , tick_clock_(new base::DefaultTickClock()) {
  Init(root_window, NULL);
}

EventGenerator::~EventGenerator() {
  for (std::list<ui::Event*>::iterator i = pending_events_.begin();
      i != pending_events_.end(); ++i)
    delete *i;
  pending_events_.clear();
  delegate()->SetContext(NULL, NULL, NULL);
}

void EventGenerator::Init(Widget* root_window, Widget* window_context) {
  delegate()->SetContext(this, root_window, window_context);
  if (window_context)
    current_location_ = delegate()->CenterOfWindow(window_context);
  current_target_ = delegate()->GetTargetAt(current_location_);
}

void EventGenerator::PressLeftButton() {
  PressButton(ui::EF_LEFT_MOUSE_BUTTON);  
}

void EventGenerator::ReleaseLeftButton() {
  ReleaseButton(ui::EF_LEFT_MOUSE_BUTTON);
}

void EventGenerator::ClickLeftButton() {
  PressLeftButton();
  ReleaseLeftButton();
}

void EventGenerator::DoubleClickLeftButton() {
  flags_ |= ui::EF_IS_DOUBLE_CLICK;
  PressLeftButton();
  flags_ ^= ui::EF_IS_DOUBLE_CLICK;
  ReleaseLeftButton();
}

void EventGenerator::Dispatch(ui::Event* event) {
  DoDispatchEvent(event, async_);
}

void EventGenerator::PressRightButton() {
  PressButton(ui::EF_RIGHT_MOUSE_BUTTON);
}

void EventGenerator::ReleaseRightButton() {
  ReleaseButton(ui::EF_RIGHT_MOUSE_BUTTON);
}

void EventGenerator::MoveMouseWheel(int delta_x, int delta_y) {
  gfx::Point location = GetLocationInCurrentRoot();
  ui::MouseEvent mouseev(ui::ET_MOUSEWHEEL, location, location, flags_, 0);
  ui::MouseWheelEvent wheelev(mouseev, delta_x, delta_y);
  Dispatch(&wheelev);
}

void EventGenerator::MoveMouseToInHost(const gfx::Point& point_in_host) {
  const ui::EventType event_type = (flags_ & ui::EF_LEFT_MOUSE_BUTTON) ?
      ui::ET_MOUSE_DRAGGED : ui::ET_MOUSE_MOVED;
  ui::MouseEvent mouseev(event_type, point_in_host, point_in_host, flags_, 0);
  Dispatch(&mouseev);

  current_location_ = point_in_host;
  delegate()->ConvertPointFromHost(current_target_, &current_location_);
}

void EventGenerator::MoveMouseTo(const gfx::Point& point_in_screen, int count) {
  DCHECK_GT(count, 0);
  const ui::EventType event_type = (flags_ & ui::EF_LEFT_MOUSE_BUTTON) ?
      ui::ET_MOUSE_DRAGGED : ui::ET_MOUSE_MOVED;

  gfx::Vector2dF diff(point_in_screen - current_location_);
  for (float i = 1; i <= count; i++) {
    gfx::Vector2dF step(diff);
    step.Scale(i / count);
    gfx::Point move_point = current_location_ + gfx::ToRoundedVector2d(step);
    if (!grab_)
      UpdateCurrentDispatcher(move_point);
    delegate()->ConvertPointToTarget(current_target_, &move_point);
    ui::MouseEvent mouseev(event_type, move_point, move_point, flags_, 0);
    Dispatch(&mouseev);
  }
  current_location_ = point_in_screen;
}

void EventGenerator::MoveMouseRelativeTo(const ui::EventTarget* window, 
                                         const gfx::Point& point_in_parent) {
  gfx::Point point(point_in_parent);
  delegate()->ConvertPointFromTarget(window, &point);
  MoveMouseTo(point);
}

void EventGenerator::DragMouseTo(const gfx::Point& point)  {
  PressLeftButton();
  MoveMouseTo(point);
  ReleaseLeftButton();
}

void EventGenerator::DispatchKeyEvent(bool is_press,
                                      ui::KeyboardCode key_code,
                                      int flags) {
#if defined(OS_WIN)
  UINT key_press = WM_KEYDOWN;
  uint16 character = ui::GetCharacterFromKeyCode(key_code, flags);
  if (is_press && character) {
    MSG native_event = { NULL, WM_KEYDOWN, key_code, 0 };
    TestKeyEvent keyev(native_event, flags);
    Dispatch(&keyev);
    // On Windows, WM_KEYDOWN event is followed by WM_CHAR with a character
    // if the key event cooresponds to a real character.
    key_press = WM_CHAR;
    key_code = static_cast<ui::KeyboardCode>(character);
  }
  MSG native_event =
      { NULL, (is_press ? key_press : WM_KEYUP), key_code, 0 };
  TestKeyEvent keyev(native_event, flags);
#elif defined(USE_X11)
  ui::ScopedXI2Event xevent;
  xevent.InitKeyEvent(is_press ? ui::ET_KEY_PRESSED : ui::ET_KEY_RELEASED,
                      key_code,
                      flags);
  ui::KeyEvent keyev(xevent);
#else
  ui::EventType type = is_press ? ui::ET_KEY_PRESSED : ui::ET_KEY_RELEASED;
  ui::KeyEvent keyev(type, key_code, flags);
#endif  // OS_WIN
  Dispatch(&keyev);
}

void EventGenerator::UpdateCurrentDispatcher(const gfx::Point& point) {
  current_target_ = delegate()->GetTargetAt(point);
}

void EventGenerator::PressButton(int flag) {
  if (!(flags_ & flag)) {
    flags_ |= flag;
    grab_ = (flags_ & kAllButtonMask) != 0;
    gfx::Point location = GetLocationInCurrentRoot();
    ui::MouseEvent mouseev(ui::ET_MOUSE_PRESSED, location, location, flags_,
                           flag);
    Dispatch(&mouseev);
  }
}

void EventGenerator::ReleaseButton(int flag) {
  if (flags_ & flag) {
    gfx::Point location = GetLocationInCurrentRoot();
    ui::MouseEvent mouseev(ui::ET_MOUSE_RELEASED, location,
                           location, flags_, flag);
    Dispatch(&mouseev);
    flags_ ^= flag;
  }
  grab_ = (flags_ & kAllButtonMask) != 0;
}

void EventGenerator::DoDispatchEvent(ui::Event* event, bool async) {
  if (async) {
    ui::Event* pending_event;
    if (event->IsKeyEvent()) {
      pending_event = new ui::KeyEvent(*static_cast<ui::KeyEvent*>(event));
    } else if (event->IsMouseEvent()) {
      pending_event = new ui::MouseEvent(*static_cast<ui::MouseEvent*>(event));
    } else if (event->IsTouchEvent()) {
      pending_event = new ui::TouchEvent(*static_cast<ui::TouchEvent*>(event));
    } else if (event->IsScrollEvent()) {
      pending_event =
          new ui::ScrollEvent(*static_cast<ui::ScrollEvent*>(event));
    } else {
      NOTREACHED() << "Invalid event type";
      return;
    }
    if (pending_events_.empty()) {
      base::ThreadTaskRunnerHandle::Get()->PostTask(
          FROM_HERE,
          base::Bind(&EventGenerator::DispatchNextPendingEvent,
                     base::Unretained(this)));
    }
    pending_events_.push_back(pending_event);
  } else {
    ui::EventSource* event_source = delegate()->GetEventSource(current_target_);
    ui::EventSourceTestApi event_source_test(event_source);
    ui::EventDispatchDetails details =
        event_source_test.SendEventToProcessor(event);
    CHECK(!details.dispatcher_destroyed);
  }
}

void EventGenerator::DispatchNextPendingEvent() {
  DCHECK(!pending_events_.empty());
  ui::Event* event = pending_events_.front();
  DoDispatchEvent(event, false);
  pending_events_.pop_front();
  delete event;
  if (!pending_events_.empty()) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::Bind(&EventGenerator::DispatchNextPendingEvent,
                   base::Unretained(this)));
  }
}

const EventGeneratorDelegate* EventGenerator::delegate() const {
  if (delegate_)
    return delegate_.get();

  DCHECK(default_delegate);
  return default_delegate;
}

EventGeneratorDelegate* EventGenerator::delegate() {
  return const_cast<EventGeneratorDelegate*>(
      const_cast<const EventGenerator*>(this)->delegate());
}
}  // namespace testing
}  // namespace widget
}  // namespace azer
