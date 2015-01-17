#include "azer/ui/views/background.h"

#include "base/logging.h"
#include "skia/ext/skia_utils_win.h"
#include "third_party/skia/include/core/SkPaint.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/color_utils.h"
#include "azer/ui/views/painter.h"
#include "azer/ui/views/view.h"

namespace azer {
namespace views {

class BackgroundPainter : public Background {
 public:
  BackgroundPainter(bool owns_painter, Painter* painter)
      : owns_painter_(owns_painter), painter_(painter) {
    DCHECK(painter);
  }

  ~BackgroundPainter() override {
    if (owns_painter_)
      delete painter_;
  }

  void Paint(gfx::Canvas* canvas, View* view) const override {
    Painter::PaintPainterAt(canvas, painter_, view->GetLocalBounds());
  }

 private:
  bool owns_painter_;
  Painter* painter_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundPainter);
};

Background::Background() {
}

Background::~Background() {
}

Background* Background::CreateBackgroundPainter(bool owns_painter,
                                                Painter* painter) {
  return new BackgroundPainter(owns_painter, painter);
}
}  // namespace views
}  // namespace azer
