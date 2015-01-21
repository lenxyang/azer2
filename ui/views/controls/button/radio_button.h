#pragma once

#include "azer/ui/views/view.h"
#include "azer/ui/views/controls/checkbox.h"

namespace azer {
namespace views {
class AZER_EXPORT RadioButton : public Checkbox {
 public:
  RadioButton(View* parent);
  ~RadioButton() override;
 private:
  DISALLOW_COPY_AND_ASSIGN(RadioButton);
};
}  // namespace views
}  // namespace azer
