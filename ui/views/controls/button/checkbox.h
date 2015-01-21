#pragma once

#include "azer/ui/views/view.h"
#include "azer/ui/views/controls/label_button.h"

namespace azer {
namespace views {
class AZER_EXPORT Checkbox : public LabelButton {
 public:
  Checkbox(View* parent);
  ~Checkbox() override;
 private:
  DISALLOW_COPY_AND_ASSIGN(Checkbox);
};
}  // namespace views
}  // namespace azer
