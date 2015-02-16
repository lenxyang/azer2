#include "azer/ui/views/controls/label.h"

namespace views {

Label::Label() {
  SetTextInternal(::base::UTF8ToWide(""));
}

Label::Label(const base::string16& text) {
  SetTextInternal(text);
}

Label::Label(const base::string16& text, const gfx::FontList& font_list) {
  SetTextInternal(text);
}

Label::~Label() {
}

void Label::SetTextInternal(const base::string16& text) {
}
}  // namespace views
