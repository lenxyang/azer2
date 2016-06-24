#pragma once

#include "ui/views/view.h"
#include "ui/views/controls/label.h"
#include "ui/gfx/geometry/insets.h"

namespace azer {
class FPSPanel : public views::View {
 public:
  FPSPanel();

  gfx::Insets GetInsets() const override;
  void Update(const FrameArgs& args);
 private:
  views::Label* avg_fps_label_;
  views::Label* cur_fps_label_;
  double last_update_time_;
  DISALLOW_COPY_AND_ASSIGN(FPSPanel);
};
}  // namespace azer
