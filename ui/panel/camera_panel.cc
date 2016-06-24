#include "azer/ui/panel/camera_panel.h"

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

CameraPanel::CameraPanel() 
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
  camera_pos_ = CreateLabel();
  camera_right_ = CreateLabel();
  camera_up_ = CreateLabel();
  camera_dir_ = CreateLabel();
  camera_vmin_ = CreateLabel();
  camera_vmax_ = CreateLabel();
  AddChildView(camera_pos_);
  AddChildView(camera_right_);
  AddChildView(camera_up_);
  AddChildView(camera_dir_);
  AddChildView(camera_vmin_);
  AddChildView(camera_vmax_);

  Layout();
}

gfx::Insets CameraPanel::GetInsets() const {
  gfx::Insets i = std::move(views::View::GetInsets());
  gfx::Insets insets(i.top() + 5,  
                     i.left() + 5,
                     i.bottom() + 5,
                     i.right() + 5);  
  return insets;
}

void CameraPanel::Update(const FrameArgs& args) {
  using base::StringPrintf;
  using base::UTF8ToUTF16;
  if (args.time() - last_update_time_ > 1.0f) {
    Vector3 v = camera.position();
    std::string posstr   = StringPrintf("Position: (%f, %f, %f)", v.x, v.y, v.z);
    v = camera.right();
    std::string rightstr = StringPrintf("Right:    (%f, %f, %f)", v.x, v.y, v.z);
    v = camera.up();
    std::string upstr    = StringPrintf("Up:       (%f, %f, %f)", v.x, v.y, v.z);
    v = camera.directional();
    std::string dirstr   = StringPrintf("Dir:      (%f, %f, %f)", v.x, v.y, v.z);
    v = vmin;
    std::string vminstr  = StringPrintf("VMin:     (%f, %f, %f)", v.x, v.y, v.z);
    v = vmax;
    std::string vmaxstr  = StringPrintf("VMax:     (%f, %f, %f)", v.x, v.y, v.z);
    
    camera_pos_->SetText(UTF8ToUTF16(postr));
    camera_right_->SetText(UTF8ToUTF16(righttr));
    camera_up_->SetText(UTF8ToUTF16(uptr));
    camera_dir_->SetText(UTF8ToUTF16(dirtr));
    camera_vmin_->SetText(UTF8ToUTF16(vmintr));
    camera_vmax_->SetText(UTF8ToUTF16(vmaxtr));
  }
}
}  // namespace lord
