#include "azer/ui/views/controls/button.h"

namespace azer {
namespace views {

Button::Button(ButtonListener* listener, View* parent)
    : View(parent)
    , listener_(listener) {
}

Button::~Button() {
}

void Button::NotifyClick(const ui::Event& event) {
  if (listener_)
    listener_->ButtonPressed(this, event);
}
}  // namespace views
}  // namespace azer
