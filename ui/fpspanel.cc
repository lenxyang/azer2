#include "azer/ui/fpspanel.h"

#include "base/logging.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/round_rect_painter.h"
#include "ui/views/shadow_border.h"
#include "ui/views/layout/box_layout.h"

namespace azer {
using views::Border;

views::Label* CreateLabel() {
  views::Label* label = new views::Label;
  label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  label->SetAutoColorReadabilityEnabled(false);
  label->SetBackgroundColor(0x00000000);
  return label;
}

FPSPanel::FPSPanel() 
    : last_update_time_(0.0) {
  SkColor border_color = SkColorSetRGB(0, 0, 0);
  std::unique_ptr<views::RoundRectPainter> painter(
      new views::RoundRectPainter(border_color, 8));
  gfx::Insets insets(1, 1, 1, 1);
  std::unique_ptr<Border> border = std::move(Border::CreateBorderPainter(
      std::move(painter), insets));
  SetBorder(std::move(border));
  
  SkColor bgcolor = SkColorSetARGB(128, 0, 0, 0);
  set_background(views::Background::CreateSolidBackground(bgcolor));
  SetLayoutManager(new views::BoxLayout(views::BoxLayout::kVertical, 0, 0, 3));
  avg_fps_label_ = CreateLabel();
  cur_fps_label_ = CreateLabel();
  AddChildView(avg_fps_label_);
  AddChildView(cur_fps_label_);
  Layout();
}

gfx::Insets FPSPanel::GetInsets() const {
  gfx::Insets i = std::move(views::View::GetInsets());
  gfx::Insets insets(i.top() + 5,  
                     i.left() + 5,
                     i.bottom() + 5,
                     i.right() + 5);  
  return insets;
}

void FPSPanel::Update(Renderer* renderer, const FrameArgs& args) {
  using base::StringPrintf;
  using base::UTF8ToUTF16;
  if (args.time() - last_update_time_ > 1.0f) {
    avg_fps_label_->SetText(UTF8ToUTF16(StringPrintf(
        "Average FPS: %f/s", args.total_average_fps())));
    cur_fps_label_->SetText(UTF8ToUTF16(StringPrintf(
        "Recent FPS: %f/s", args.recent_average_fps())));
    last_update_time_ = args.time();
  }
}
}  // namespace lord
