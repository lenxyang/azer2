#include "azer/ui/panel/camera_panel.h"

#include "base/logging.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/round_rect_painter.h"
#include "ui/views/shadow_border.h"
#include "ui/views/layout/box_layout.h"

#include "azer/render/camera.h"
#include "azer/render/frame_args.h"

namespace azer {
using views::Border;

namespace {
views::Label* CreateLabel() {
  views::Label* label = new views::Label;
  label->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  label->SetAutoColorReadabilityEnabled(false);
  label->SetBackgroundColor(0x00000000);
  return label;
}
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
  camera_near_lefttop_ = CreateLabel();
  camera_near_rightbottom_ = CreateLabel();
  camera_far_lefttop_ = CreateLabel();
  camera_far_rightbottom_ = CreateLabel();
  camera_vmin_ = CreateLabel();
  camera_vmax_ = CreateLabel();
  
  AddChildView(camera_pos_);
  AddChildView(camera_right_);
  AddChildView(camera_up_);
  AddChildView(camera_dir_);
  AddChildView(camera_vmin_);
  AddChildView(camera_vmax_);
  AddChildView(camera_near_lefttop_);
  AddChildView(camera_near_rightbottom_);
  AddChildView(camera_far_lefttop_);
  AddChildView(camera_far_rightbottom_);

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

#define SET_LABEL_VERTEX(view, title, vertex)                    \
  view->SetText(UTF8ToUTF16(StringPrintf("%s: (%.3f, %.3f, %.3f)", (title),(vertex).x, (vertex).y, (vertex).z)));
  

void CameraPanel::Update(const Camera& camera, const FrameArgs& args) {
  using base::StringPrintf;
  using base::UTF8ToUTF16;
  if (args.time() - last_update_time_ > 0.2f) {
    last_update_time_ = args.time();
    Vector3 vmin, vmax;
    Vector3 boundpos[8];
    CalcCameraBundingBox(camera, camera.frustum().get_near(),
                         camera.frustum().get_far(), &vmin, &vmax);
    CalcCameraBundingPos(camera, boundpos);
    Vector3 v = camera.position();
    SET_LABEL_VERTEX(camera_pos_,   "Position: ", camera.position());
    SET_LABEL_VERTEX(camera_right_, "Right:    ", camera.right());
    SET_LABEL_VERTEX(camera_up_,    "Up:       ", camera.up());
    SET_LABEL_VERTEX(camera_dir_,   "Dir:      ", camera.directional());
    SET_LABEL_VERTEX(camera_vmin_,  "VMin:     ", vmin);
    SET_LABEL_VERTEX(camera_vmax_,  "VMax:     ", vmax);
    SET_LABEL_VERTEX(camera_near_lefttop_, "NearLT:  ", boundpos[0]);
    SET_LABEL_VERTEX(camera_near_rightbottom_, "NearRB  ", boundpos[2]);
    SET_LABEL_VERTEX(camera_far_lefttop_, "FarLT:   ", boundpos[4]);
    SET_LABEL_VERTEX(camera_far_rightbottom_, "FarRB   ", boundpos[6]);
  }
}
}  // namespace lord
