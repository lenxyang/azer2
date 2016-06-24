#pragma once

#include "ui/views/view.h"
#include "ui/views/controls/label.h"
#include "ui/gfx/geometry/insets.h"

namespace azer {
class Camera;
class CameraPanel : public views::View {
 public:
  CameraPanel();

  gfx::Insets GetInsets() const override;
  void Update(const Camera& camera, const FrameArgs& args);
 private:
  views::Label* camera_pos_;
  views::Label* camera_right_;
  views::Label* camera_up_;
  views::Label* camera_dir_;
  views::Label* camera_vmin_;
  views::Label* camera_vmax_;
  double last_update_time_;
  DISALLOW_COPY_AND_ASSIGN(CameraPanel);
};
}  // namespace azer
