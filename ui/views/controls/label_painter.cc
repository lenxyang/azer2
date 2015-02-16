#include "azer/ui/views/controls/label_painter.h"

namespace views {
void LabelPainter::Paint(gfx::Canvas* canvas, Label* label) {
}

void LabelPainter::CalculateDrawStringParams(base::string16* paint_text,
                                             gfx::Rect* text_bounds,
                                             int* flags) const {
  DCHECK(paint_text && text_bounds && flags);

  const bool forbid_ellipsis = elide_behavior_ == gfx::NO_ELIDE ||
                               elide_behavior_ == gfx::FADE_TAIL;
  if (multi_line_ || forbid_ellipsis) {
    *paint_text = layout_text_;
  } else {
    *paint_text = gfx::ElideText(layout_text_, font_list_,
                                 GetAvailableRect().width(), elide_behavior_);
  }

  *text_bounds = GetTextBounds();
  *flags = ComputeDrawStringFlags();
  // TODO(msw): Elide multi-line text with ElideRectangleText instead.
  if (!multi_line_ || forbid_ellipsis)
    *flags |= gfx::Canvas::NO_ELLIPSIS;
}

void LabelPainter::ResetCachedSize() {
  text_size_valid_ = false;
  cached_heights_cursor_ = 0;
  for (int i = 0; i < kCachedSizeLimit; ++i)
    cached_heights_[i] = gfx::Size();
}

void LabelPainter::PaintText(gfx::Canvas* canvas,
                             const base::string16& text,
                             const gfx::Rect& text_bounds,
                             int flags) {
  DCHECK(label_);
  gfx::ElideBehavior elide_behavior = label_->param().elide_behavior;
  const gfx::FontList& font_list = label_->param().font_list;
  int32 line_height = label_->param().line_height;
  const gfx::ShadowValues& shadows = label_->param().shadows();
  SkColor color = label_->enabled() ? actual_enabled_color_ : actual_disabled_color_;
  if (elide_behavior == gfx::FADE_TAIL) {
    canvas->DrawFadedString(text, font_list, color, text_bounds, flags);
  } else {
    canvas->DrawStringRectWithShadows(text, font_list, color, text_bounds,
                                      line_height, flags, shadows);
  }

  if (HasFocus()) {
    gfx::Rect focus_bounds = text_bounds;
    focus_bounds.Inset(-kFocusBorderPadding, -kFocusBorderPadding);
    canvas->DrawFocusRect(focus_bounds);
  }
}
}  // namespace views
