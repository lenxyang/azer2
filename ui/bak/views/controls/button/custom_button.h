#pragma once

#include "ui/events/event_constants.h"
#include "azer/ui/views/controls/button/button.h"

namespace gfx {
class ThrobAnimation;
}

namespace azer {
namespace views {

class CustomButtonStateChangedDelegate;

class AZER_EXPORT CustomButton : public Button {
 public:
  CustomButton(ButtonListener* listener, View* view);
  ~CustomButton() override;

  // The menu button's class name.
  static const char kViewClassName[];

  void SetState(ButtonState state);
  void OnEnabledChanged() override;   // override from View

  bool OnMousePressed(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnMouseCaptureLost() override;
  void OnMouseEntered(const ui::MouseEvent& event) override;
  void OnMouseExited(const ui::MouseEvent& event) override;
  void OnMouseMoved(const ui::MouseEvent& event) override;
  bool OnKeyPressed(const ui::KeyEvent& event) override;
  bool OnKeyReleased(const ui::KeyEvent& event) override;
 private:
  // Invoked from SetState() when SetState() is passed a value that differs from
  // the current state. CustomButton's implementation of StateChanged() does
  // nothing; this method is provided for subclasses that wish to do something
  // on state changes.
  virtual void StateChanged();

  // Returns true if the event is one that can trigger notifying the listener.
  // This implementation returns true if the left mouse button is down.
  virtual bool IsTriggerableEvent(const ui::Event& event);

// Returns true if the button should become pressed when the user
  // holds the mouse down over the button. For this implementation,
  // we simply return IsTriggerableEvent(event).
  virtual bool ShouldEnterPushedState(const ui::Event& event);

  // The button state (defined in implementation)
  ButtonState state_;

  // Hover animation.
  scoped_ptr<gfx::ThrobAnimation> hover_animation_;

  bool animate_on_state_change_;
  bool is_throbbing_;
  bool triggerable_event_flags_;
  bool request_focus_on_press_;

  scoped_ptr<CustomButtonStateChangedDelegate> state_changed_delegate_;
  DISALLOW_COPY_AND_ASSIGN(CustomButton);
};

// Delegate for actions taken on state changes by CustomButton.
class AZER_EXPORT CustomButtonStateChangedDelegate {
public:
  virtual ~CustomButtonStateChangedDelegate() {}
  virtual void StateChanged(Button::ButtonState state) = 0;

protected:
  CustomButtonStateChangedDelegate() {}

private:
  DISALLOW_COPY_AND_ASSIGN(CustomButtonStateChangedDelegate);
};
}  // namespace views
}  // namespace azer
