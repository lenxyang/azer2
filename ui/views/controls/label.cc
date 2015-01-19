#include "azer/ui/views/controls/label.h"

namespace azer {
namespace views {

Label::Label() {
}

Label::~Label() {
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
}  // namespace views
}  // namespace azer
