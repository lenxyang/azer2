#pragma once

#include "nelf/nelf.h"
#include "ui/views/view.h"

namespace azer {
class FPSPanel : public views::View {
 public:
  FPSPanel();

  gfx::Insets GetInsets() const override;
  void Update(azer::Renderer* renderer, const azer::FrameArgs& args);
 private:
  views::Label* avg_fps_label_;
  views::Label* cur_fps_label_;
  double last_update_time_;
  DISALLOW_COPY_AND_ASSIGN(FPSPanel);
};
}  // namespace azer
