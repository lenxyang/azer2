#include "azer/ui/views/controls/label_painter.h"

#include "base/i18n/rtl.h"
#include "base/logging.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/text_elider.h"
#include "ui/gfx/text_utils.h"
#include "ui/gfx/utf16_indexing.h"

namespace views {
void LabelPainter::Paint(gfx::Canvas* canvas, Label* label) {
  base::string16 paint_text;
  gfx::Rect text_bounds;
  int flags = 0;
  CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  PaintText(canvas, paint_text, text_bounds, flags);
}

gfx::Rect LabelPainter::GetTextBounds() const {
  gfx::Rect available(label_->GetAvailableRect());
  gfx::Size text_size(GetTextSize());
  text_size.set_width(std::min(available.width(), text_size.width()));
  gfx::Point origin(label_->GetInsets().left(), label_->GetInsets().top());
  switch (GetHorizontalAlignment()) {
    case gfx::ALIGN_LEFT:
      break;
    case gfx::ALIGN_CENTER:
      // Put any extra margin pixel on the left to match the legacy behavior
      // from the use of GetTextExtentPoint32() on Windows.
      origin.Offset((available.width() + 1 - text_size.width()) / 2, 0);
      break;
    case gfx::ALIGN_RIGHT:
      origin.set_x(available.right() - text_size.width());
      break;
    default:
      NOTREACHED();
      break;
  }
  if (!multi_line_)
    text_size.set_height(available.height());
  // Support vertical centering of multi-line labels: http://crbug.com/429595
  origin.Offset(0, std::max(0, (available.height() - text_size.height())) / 2);
  return gfx::Rect(origin, text_size);
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

  if (label_->HasFocus()) {
    gfx::Rect focus_bounds = text_bounds;
    focus_bounds.Inset(-kFocusBorderPadding, -kFocusBorderPadding);
    canvas->DrawFocusRect(focus_bounds);
  }
}
}  // namespace views
