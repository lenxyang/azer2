#pragma once


#include "azer/ui/views/controls/button/custom_button.h"

namespace azer {
namespace views {
class AZER_EXPORT LabelButton : public CustomButton {
 public:
  LabelButton(ButtonListener* listener, View* view);
  ~LabelButton() override;
 private:
  DISALLOW_COPY_AND_ASSIGN(LabelButton);
};
}  // namespace views
}  // namespace azer
