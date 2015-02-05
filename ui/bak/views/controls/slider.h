#pragma once

#include "azer/ui/views/view.h"

namespace azer {
namespace views {
class AZER_EXPORT Slider : public View {
 public:
  Slider(View* parent);
  ~Slider() override;
 private:
  DISALLOW_COPY_AND_ASSIGN(Slider);
};
}  // namespace views
}  // namespace azer
