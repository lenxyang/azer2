#include "azer/ui/views/controls/button/custom_button.h"

#include "ui/events/event.h"
#include "ui/events/keycodes/keyboard_codes.h"
#include "ui/gfx/animation/throb_animation.h"
#include "ui/gfx/screen.h"

namespace azer {
namespace views {
// static
const char CustomButton::kViewClassName[] = "CustomButton";

CustomButton::CustomButton(ButtonListener* listener, View* parent)
    : Button(listener, parent) 
    , state_(STATE_NORMAL)
    , animate_on_state_change_(true)
    , is_throbbing_(false)
    , triggerable_event_flags_(ui::EF_LEFT_MOUSE_BUTTON)
    , request_focus_on_press_(true)  {
}

CustomButton::~CustomButton() {
}

void CustomButton::StateChanged() {
}

bool CustomButton::IsTriggerableEvent(const ui::Event& event) {
  return event.type() == ui::ET_GESTURE_TAP_DOWN ||
         event.type() == ui::ET_GESTURE_TAP ||
         (event.IsMouseEvent() &&
             (triggerable_event_flags_ & event.flags()) != 0);
}

void CustomButton::SetState(ButtonState state) {
  if (state == state_)
    return;

  StateChanged();
  if (state_changed_delegate_.get())
    state_changed_delegate_->StateChanged(state_);
  SchedulePaint();
}

void CustomButton::OnEnabledChanged() {
  if (enabled() ? (state_ != STATE_DISABLED) : (state_ == STATE_DISABLED))
    return;

  if (enabled())
    SetState(IsMouseHovered() ? STATE_HOVERED : STATE_NORMAL);
  else
    SetState(STATE_DISABLED);
}

bool CustomButton::OnMousePressed(const ui::MouseEvent& event) {
  if (state_ != STATE_DISABLED) {
    if (ShouldEnterPushedState(event) && HitTestPoint(event.location()))
      SetState(STATE_PRESSED);
    if (request_focus_on_press_)
      RequestFocus();
  }
  return true;
}

bool CustomButton::OnMouseDragged(const ui::MouseEvent& event) {
  if (state_ != STATE_DISABLED) {
    if (HitTestPoint(event.location()))
      SetState(ShouldEnterPushedState(event) ? STATE_PRESSED : STATE_HOVERED);
    else
      SetState(STATE_NORMAL);
  }
  return true;
}

void CustomButton::OnMouseReleased(const ui::MouseEvent& event) {
  if (state_ == STATE_DISABLED)
    return;

  if (!HitTestPoint(event.location())) {
    SetState(STATE_NORMAL);
    return;
  }

  SetState(STATE_HOVERED);
  if (IsTriggerableEvent(event)) {
    NotifyClick(event);
    // NOTE: We may be deleted at this point (by the listener's notification
    // handler).
  }
}

void CustomButton::OnMouseCaptureLost() {
  // Starting a drag results in a MouseCaptureLost, we need to ignore it.
  if (state_ != STATE_DISABLED && !InDrag())
    SetState(STATE_NORMAL);
}

void CustomButton::OnMouseEntered(const ui::MouseEvent& event) {
  if (state_ != STATE_DISABLED)
    SetState(STATE_HOVERED);
}

void CustomButton::OnMouseExited(const ui::MouseEvent& event) {
  // Starting a drag results in a MouseExited, we need to ignore it.
  if (state_ != STATE_DISABLED && !InDrag())
    SetState(STATE_NORMAL);
}

void CustomButton::OnMouseMoved(const ui::MouseEvent& event) {
  if (state_ != STATE_DISABLED)
    SetState(HitTestPoint(event.location()) ? STATE_HOVERED : STATE_NORMAL);
}

bool CustomButton::OnKeyPressed(const ui::KeyEvent& event) {
  if (state_ == STATE_DISABLED)
    return false;

  // Space sets button state to pushed. Enter clicks the button. This matches
  // the Windows native behavior of buttons, where Space clicks the button on
  // KeyRelease and Enter clicks the button on KeyPressed.
  if (event.key_code() == ui::VKEY_SPACE) {
    SetState(STATE_PRESSED);
  } else if (event.key_code() == ui::VKEY_RETURN) {
    SetState(STATE_NORMAL);
    // TODO(beng): remove once NotifyClick takes ui::Event.
    ui::MouseEvent synthetic_event(ui::ET_MOUSE_RELEASED,
                                   gfx::Point(),
                                   gfx::Point(),
                                   ui::EF_LEFT_MOUSE_BUTTON,
                                   ui::EF_LEFT_MOUSE_BUTTON);
    NotifyClick(synthetic_event);
  } else {
    return false;
  }
  return true;
}

bool CustomButton::OnKeyReleased(const ui::KeyEvent& event) {
  if ((state_ == STATE_DISABLED) || (event.key_code() != ui::VKEY_SPACE))
    return false;

  SetState(STATE_NORMAL);
  // TODO(beng): remove once NotifyClick takes ui::Event.
  ui::MouseEvent synthetic_event(ui::ET_MOUSE_RELEASED,
                                 gfx::Point(),
                                 gfx::Point(),
                                 ui::EF_LEFT_MOUSE_BUTTON,
                                 ui::EF_LEFT_MOUSE_BUTTON);
  NotifyClick(synthetic_event);
  return true;
}

}  // namespace views
}  // namespace azer
