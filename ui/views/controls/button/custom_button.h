#pragma once

#include "azer/ui/views/controls/button/button.h"

namespace azer {
namespace views {
class AZER_EXPORT CustomButton : public Button {
 public:
  CustomButton(View* view);
  ~CustomButton() override;

  // The menu button's class name.
  static const char kViewClassName[];
 private:
  DISALLOW_COPY_AND_ASSIGN(CustomButton);
};
}  // namespace views
}  // namespace azer
