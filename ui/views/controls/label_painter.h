#pragma once


#include "ui/base/cursor/cursor.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/text_constants.h"
#include "ui/gfx/geometry/rect.h"
#include "azer/ui/widget/controls/label.h"

namespace widget {
class WIDGET_EXPORT LabelPainter {
 public:
  LabelPainter();
  ~LabelPainter();

  void Paint(gfx::Canvas* canvas, const Label::Params& param);
 private:
  mutable std::vector<gfx::Size> cached_heights_;
  mutable int cached_heights_cursor_;
  DISALLOW_COPY_AND_ASSIGN(LabelPainter);
};
}  // namespace widget
