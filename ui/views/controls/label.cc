#include "azer/ui/views/controls/label.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

#include "base/i18n/rtl.h"
#include "base/logging.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "ui/accessibility/ax_view_state.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/text_elider.h"
#include "ui/gfx/text_utils.h"
#include "ui/gfx/utf16_indexing.h"
#include "ui/views/background.h"

namespace azer {
namespace views {

namespace {

const int kCachedSizeLimit = 10;
const base::char16 kPasswordReplacementChar = '*';

}  // namespace

const char Label::kViewClassName[] = "Label";
const int Label::kFocusBorderPadding = 1;

Label::Label() {
}

Label::~Label() {
}

void Label::Init(const base::string16& text, const gfx::FontList& font_list) {
  font_list_ = font_list;
  enabled_color_set_ = disabled_color_set_ = background_color_set_ = false;
  subpixel_rendering_enabled_ = true;
  auto_color_readability_ = true;

  horizontal_alignment_ = gfx::ALIGN_CENTER;
  line_height_ = 0;
  multi_line_ = false;
  obscured_ = false;
  allow_character_break_ = false;
  elide_behavior_ = gfx::ELIDE_TAIL;
  collapse_when_hidden_ = false;

  SetText(text);
}

void Label::SetLineHeight(int height) {
}

void Label::SetMultiLine(bool multi_line) {
}

void Label::SetObscured(bool obscured) {
}

void Label::SetText(const base::string16& text) {
}

void Label::SetFontList(const gfx::FontList& font_list) {
}

void Label::SetEnabledColor(SkColor color) {
  requested_enabled_color_ = color;
  enabled_color_set_ = true;
  RecalculateColors();
}

void Label::SetDisabledColor(SkColor color) {
  requested_disabled_color_ = color;
  disabled_color_set_ = true;
  RecalculateColors();
}

void Label::SetBackgroundColor(SkColor color) {
  background_color_ = color;
  background_color_set_ = true;
  RecalculateColors();
}

void Label::SetShadows(const gfx::ShadowValues& shadows) {
  shadows_ = shadows;
  text_size_valid_ = false;
}

void Label::SetSubpixelRenderingEnabled(bool subpixel_rendering_enabled) {
  subpixel_rendering_enabled_ = subpixel_rendering_enabled;
}

void Label::SetHorizontalAlignment(gfx::HorizontalAlignment alignment) {
  // If the UI layout is right-to-left, flip the alignment direction.
  if (base::i18n::IsRTL() &&
      (alignment == gfx::ALIGN_LEFT || alignment == gfx::ALIGN_RIGHT)) {
    alignment = (alignment == gfx::ALIGN_LEFT) ?
        gfx::ALIGN_RIGHT : gfx::ALIGN_LEFT;
  }
  if (horizontal_alignment_ != alignment) {
    horizontal_alignment_ = alignment;
    SchedulePaint();
  }
}

gfx::HorizontalAlignment Label::GetHorizontalAlignment() const {
  if (horizontal_alignment_ != gfx::ALIGN_TO_HEAD)
    return horizontal_alignment_;

  const base::i18n::TextDirection dir =
      base::i18n::GetFirstStrongCharacterDirection(layout_text_);
  return dir == base::i18n::RIGHT_TO_LEFT ? gfx::ALIGN_RIGHT : gfx::ALIGN_LEFT;
}

void Label::OnPaint(gfx::Canvas* canvas) {
  OnPaintBackground(canvas);
  // We skip painting the focus border because it is being handled seperately by
  // some subclasses of Label. We do not want View's focus border painting to
  // interfere with that.
  OnPaintBorder(canvas);

  base::string16 paint_text;
  gfx::Rect text_bounds;
  int flags = 0;
  CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  PaintText(canvas, paint_text, text_bounds, flags);
}

void Label::PaintText(gfx::Canvas* canvas,
                      const base::string16& text,
                      const gfx::Rect& text_bounds,
                      int flags) {
  SkColor color = enabled() ? actual_enabled_color_ : actual_disabled_color_;
  if (elide_behavior_ == gfx::FADE_TAIL) {
    canvas->DrawFadedString(text, font_list_, color, text_bounds, flags);
  } else {
    canvas->DrawStringRectWithShadows(text, font_list_, color, text_bounds,
                                      line_height_, flags, shadows_);
  }

  if (HasFocus()) {
    gfx::Rect focus_bounds = text_bounds;
    focus_bounds.Inset(-kFocusBorderPadding, -kFocusBorderPadding);
    canvas->DrawFocusRect(focus_bounds);
  }
}

gfx::Size Label::GetTextSize() const {
  if (!text_size_valid_) {
    // For single-line strings, we supply the largest possible width, because
    // while adding NO_ELLIPSIS to the flags works on Windows for forcing
    // SizeStringInt() to calculate the desired width, it doesn't seem to work
    // on Linux.
    int w = multi_line_ ?
        GetAvailableRect().width() : std::numeric_limits<int>::max();
    int h = font_list_.GetHeight();
    // For single-line strings, ignore the available width and calculate how
    // wide the text wants to be.
    int flags = ComputeDrawStringFlags();
    if (!multi_line_)
      flags |= gfx::Canvas::NO_ELLIPSIS;
    gfx::Canvas::SizeStringInt(
        layout_text_, font_list_, &w, &h, line_height_, flags);
    text_size_.SetSize(w, h);
    const gfx::Insets shadow_margin = -gfx::ShadowValue::GetMargin(shadows_);
    text_size_.Enlarge(shadow_margin.width(), shadow_margin.height());
    text_size_valid_ = true;
  }

  return text_size_;
}

void Label::RecalculateColors() {
  actual_enabled_color_ = auto_color_readability_ ?
      color_utils::GetReadableColor(requested_enabled_color_,
                                    background_color_) :
      requested_enabled_color_;
  actual_disabled_color_ = auto_color_readability_ ?
      color_utils::GetReadableColor(requested_disabled_color_,
                                    background_color_) :
      requested_disabled_color_;
}

gfx::Rect Label::GetTextBounds() const {
  gfx::Rect available(GetAvailableRect());
  gfx::Size text_size(GetTextSize());
  text_size.set_width(std::min(available.width(), text_size.width()));
  gfx::Point origin(GetInsets().left(), GetInsets().top());
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

int Label::ComputeDrawStringFlags() const {
  int flags = 0;

  // We can't use subpixel rendering if the background is non-opaque.
  if (SkColorGetA(background_color_) != 0xFF || !subpixel_rendering_enabled_)
    flags |= gfx::Canvas::NO_SUBPIXEL_RENDERING;

  base::i18n::TextDirection direction =
      base::i18n::GetFirstStrongCharacterDirection(layout_text_);
  if (direction == base::i18n::RIGHT_TO_LEFT)
    flags |= gfx::Canvas::FORCE_RTL_DIRECTIONALITY;
  else
    flags |= gfx::Canvas::FORCE_LTR_DIRECTIONALITY;

  switch (GetHorizontalAlignment()) {
    case gfx::ALIGN_LEFT:
      flags |= gfx::Canvas::TEXT_ALIGN_LEFT;
      break;
    case gfx::ALIGN_CENTER:
      flags |= gfx::Canvas::TEXT_ALIGN_CENTER;
      break;
    case gfx::ALIGN_RIGHT:
      flags |= gfx::Canvas::TEXT_ALIGN_RIGHT;
      break;
    default:
      NOTREACHED();
      break;
  }

  if (!multi_line_)
    return flags;

  flags |= gfx::Canvas::MULTI_LINE;
#if !defined(OS_WIN)
    // Don't elide multiline labels on Linux.
    // Todo(davemoore): Do we depend on eliding multiline text?
    // Pango insists on limiting the number of lines to one if text is
    // elided. You can get around this if you can pass a maximum height
    // but we don't currently have that data when we call the pango code.
    flags |= gfx::Canvas::NO_ELLIPSIS;
#endif
  if (allow_character_break_)
    flags |= gfx::Canvas::CHARACTER_BREAK;

  return flags;
}

gfx::Rect Label::GetAvailableRect() const {
  gfx::Rect bounds(size());
  bounds.Inset(GetInsets());
  return bounds;
}

void Label::CalculateDrawStringParams(base::string16* paint_text,
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
}  // namespace views
}  // namespace azer
