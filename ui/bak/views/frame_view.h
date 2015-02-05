#pragma once

#include "base/compiler_specific.h"
#include "azer/ui/views/view.h"

namespace azer {
namespace views {

class AZER_EXPORT FrameView: public View {
 public:
  explicit FrameView(View* parent);
  ~FrameView() override;
 private:
  void PaintCaptionBar(gfx::Canvas* canvas);
  void PaintSystemMenu(gfx::Canvas* canvas);
  DISALLOW_COPY_AND_ASSIGN(FrameView);
};
}  // namespace views
}  // namespace azer
