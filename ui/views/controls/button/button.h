#pragma once

#include "ui/events/event.h"
#include "azer/ui/views/controls/control.h"

namespace azer {
namespace views {

class AZER_EXPORT ButtonListener {
 public:
  virtual void ButtonPressed(Button* sender, const ui::Event& event) = 0;
 protected:
  virtual ~ButtonListener() {}
};

class AZER_EXPORT Button : public Control {
 public:
  ~Button() override;

  // Button states for various button sub-types.
  enum ButtonState {
    STATE_NORMAL = 0,
    STATE_HOVERED,
    STATE_PRESSED,
    STATE_DISABLED,
    STATE_COUNT,
  };

  // Button styles with associated images and border painters.
  // TODO(msw): Add Menu, ComboBox, etc.
  enum ButtonStyle {
    STYLE_BUTTON = 0,
    STYLE_TEXTBUTTON,
    STYLE_COUNT,
  };

  static ButtonState GetButtonStateFrom(ui::NativeTheme::State state);
 protected:
  Button(ButtonListener* listener, View* view);

  // Cause the button to notify the listener that a click occurred.
  virtual void NotifyClick(const ui::Event& event);

  // The button's listener. Notified when clicked.
  ButtonListener* listener_;
 private:
  DISALLOW_COPY_AND_ASSIGN(Button);
};
}  // namespace views
}  // namespace azer
