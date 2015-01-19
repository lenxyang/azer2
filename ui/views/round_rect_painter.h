#pragma once

#include "ui/gfx/rect.h"
#include "ui/views/painter.h"
#include "ui/views/views_export.h"

namespace gfx {
class Canvas;
class Path;
class Size;
}

namespace azer {
namespace views {

// Painter to draw a border with rounded corners.
class AZER_EXPORT RoundRectPainter : public Painter {
 public:
  RoundRectPainter(SkColor border_color, int corner_radius);
  ~RoundRectPainter() override;

  // Painter:
  gfx::Size GetMinimumSize() const override;
  void Paint(gfx::Canvas* canvas, const gfx::Size& size) override;

 private:
  const SkColor border_color_;
  const int corner_radius_;

  DISALLOW_COPY_AND_ASSIGN(RoundRectPainter);
};

}  // namespace views
}  // namespace azer


