#pragma once

#include <vector>

#include "ui/base/cursor/cursor.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/text_constants.h"
#include "ui/gfx/geometry/rect.h"
#include "azer/ui/views/controls/label.h"

namespace views {
class VIEWS_EXPORT LabelPainter {
 public:
  LabelPainter();
  ~LabelPainter();

  void Paint(gfx::Canvas* canvas, Label* label);
 private:
  void CalculateDrawStringParams(base::string16* paint_text,
                                 gfx::Rect* text_bounds,
                                 int* flags) const;
  void PaintText(gfx::Canvas* canvas, const base::string16& text,
                 const gfx::Rect& text_bounds, int flags);
  gfx::Rect GetTextBounds() const;

  void ResetCachedSize();
  int ComputeDrawStringFlags() const;
  mutable std::vector<gfx::Size> cached_heights_;
  mutable int cached_heights_cursor_;
  Label* label_;
  DISALLOW_COPY_AND_ASSIGN(LabelPainter);
};
}  // namespace views
