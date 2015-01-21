#include "azer/ui/views/controls/button/label_button.h"

namespace azer {
namespace views {

LabelButton::LabelButton(ButtonListener* listener, View* parent)
    : CustomButton(listener, parent) {
}

LabelButton::~LabelButton() {
}
}  // namespace views
}  // namespace azer
