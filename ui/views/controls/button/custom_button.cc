#include "azer/ui/views/controls/custom_button.h"

namespace azer {
namespace views {
// static
const char CustomButton::kViewClassName[] = "CustomButton";

CustomButton::CustomButton(View* parent)
    : Button(parent) {
}

CustomButton::~CustomButton() {
}


}  // namespace views
}  // namespace azer
