#pragma once

#include "azer/ui/views/view.h"

namespace azer {
namespace views {
class AZER_EXPORT LabelButton : public View {
 public:
  LabelButton(View* view);
  ~LabelButton() override;
 private:
  DISALLOW_COPY_AND_ASSIGN(LabelButton);
};
}  // namespace views
}  // namespace azer
