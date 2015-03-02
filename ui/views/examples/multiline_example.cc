// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "azer/ui/views/examples/multiline_example.h"

#include "base/strings/utf_string_conversions.h"
#include "ui/events/event.h"
#include "ui/gfx/render_text.h"
#include "azer/ui/views/background.h"
#include "azer/ui/views/border.h"
#include "azer/ui/views/controls/button/checkbox.h"
#include "azer/ui/views/controls/label.h"
#include "azer/ui/views/controls/textfield/textfield.h"
#include "azer/ui/views/layout/grid_layout.h"
#include "azer/ui/views/view.h"

using base::ASCIIToUTF16;

namespace views {
namespace examples {

namespace {

gfx::Range ClampRange(gfx::Range range, size_t max) {
  range.set_start(std::min(range.start(), max));
  range.set_end(std::min(range.end(), max));
  return range;
}

// A Label with a clamped preferred width to demonstrate wrapping.
class PreferredSizeLabel : public Label {
 public:
  PreferredSizeLabel() : Label() {}
  ~PreferredSizeLabel() override {}

  // Label:
  gfx::Size GetPreferredSize() const override {
    return gfx::Size(50, Label::GetPreferredSize().height());
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(PreferredSizeLabel);
};

}  // namespace

// A simple View that hosts a RenderText object.
class MultilineExample::RenderTextView : public View {
 public:
  RenderTextView() : render_text_(gfx::RenderText::CreateInstance()) {
    render_text_->SetHorizontalAlignment(gfx::ALIGN_CENTER);
    render_text_->SetColor(SK_ColorBLACK);
    render_text_->SetMultiline(true);
    SetBorder(Border::CreateSolidBorder(2, SK_ColorGRAY));
  }

  void OnPaint(gfx::Canvas* canvas) override {
    View::OnPaint(canvas);
    render_text_->Draw(canvas);
  }

  gfx::Size GetPreferredSize() const override {
    // Turn off multiline mode to get the single-line text size, which is the
    // preferred size for this view.
    render_text_->SetMultiline(false);
    gfx::Size size(render_text_->GetContentWidth(),
                   render_text_->GetStringSize().height());
    size.Enlarge(GetInsets().width(), GetInsets().height());
    render_text_->SetMultiline(true);
    return size;
  }

  int GetHeightForWidth(int w) const override {
    // TODO(ckocagil): Why does this happen?
    if (w == 0)
      return View::GetHeightForWidth(w);
    const gfx::Rect old_rect = render_text_->display_rect();
    gfx::Rect rect = old_rect;
    rect.set_width(w - GetInsets().width());
    render_text_->SetDisplayRect(rect);
    int height = render_text_->GetStringSize().height() + GetInsets().height();
    render_text_->SetDisplayRect(old_rect);
    return height;
  }

  void SetText(const base::string16& new_contents) {
    // Color and style the text inside |test_range| to test colors and styles.
    const size_t range_max = new_contents.length();
    gfx::Range color_range = ClampRange(gfx::Range(1, 21), range_max);
    gfx::Range bold_range = ClampRange(gfx::Range(4, 10), range_max);
    gfx::Range italic_range = ClampRange(gfx::Range(7, 13), range_max);

    render_text_->SetText(new_contents);
    render_text_->SetColor(SK_ColorBLACK);
    render_text_->ApplyColor(0xFFFF0000, color_range);
    render_text_->SetStyle(gfx::DIAGONAL_STRIKE, false);
    render_text_->ApplyStyle(gfx::DIAGONAL_STRIKE, true, color_range);
    render_text_->SetStyle(gfx::UNDERLINE, false);
    render_text_->ApplyStyle(gfx::UNDERLINE, true, color_range);
    render_text_->ApplyStyle(gfx::BOLD, true, bold_range);
    render_text_->ApplyStyle(gfx::ITALIC, true, italic_range);
    InvalidateLayout();
  }

 private:
  void OnBoundsChanged(const gfx::Rect& previous_bounds) override {
    gfx::Rect bounds = GetLocalBounds();
    bounds.Inset(GetInsets());
    render_text_->SetDisplayRect(bounds);
  }

  scoped_ptr<gfx::RenderText> render_text_;

  DISALLOW_COPY_AND_ASSIGN(RenderTextView);
};

MultilineExample::MultilineExample()
    : ExampleBase("Multiline RenderText"),
      render_text_view_(NULL),
      label_(NULL),
      textfield_(NULL),
      label_checkbox_(NULL) {
}

MultilineExample::~MultilineExample() {
}

void MultilineExample::CreateExampleView(View* container) {
  const base::string16 kTestString = base::WideToUTF16(L"qwerty"
      L"\x627\x644\x631\x626\x64A\x633\x64A\x629"
      L"asdfgh");

  render_text_view_ = new RenderTextView();
  render_text_view_->SetText(kTestString);

  label_ = new PreferredSizeLabel();
  label_->SetText(kTestString);
  label_->SetMultiLine(true);
  label_->SetBorder(Border::CreateSolidBorder(2, SK_ColorCYAN));

  label_checkbox_ = new Checkbox(ASCIIToUTF16("views::Label:"));
  label_checkbox_->SetChecked(true);
  label_checkbox_->set_listener(this);
  label_checkbox_->set_request_focus_on_press(false);

  textfield_ = new Textfield();
  textfield_->set_controller(this);
  textfield_->SetText(kTestString);

  GridLayout* layout = new GridLayout(container);
  container->SetLayoutManager(layout);

  ColumnSet* column_set = layout->AddColumnSet(0);
  column_set->AddColumn(GridLayout::LEADING, GridLayout::CENTER,
      0.0f, GridLayout::USE_PREF, 0, 0);
  column_set->AddColumn(GridLayout::FILL, GridLayout::FILL,
      1.0f, GridLayout::FIXED, 0, 0);

  layout->StartRow(0, 0);
  layout->AddView(new Label(ASCIIToUTF16("gfx::RenderText:")));
  layout->AddView(render_text_view_);

  layout->StartRow(0, 0);
  layout->AddView(label_checkbox_);
  layout->AddView(label_);

  layout->StartRow(0, 0);
  layout->AddView(new Label(ASCIIToUTF16("Sample Text:")));
  layout->AddView(textfield_);
}

void MultilineExample::ContentsChanged(Textfield* sender,
                                       const base::string16& new_contents) {
  render_text_view_->SetText(new_contents);
  if (label_checkbox_->checked())
    label_->SetText(new_contents);
  container()->Layout();
  container()->SchedulePaint();
}

void MultilineExample::ButtonPressed(Button* sender, const ui::Event& event) {
  DCHECK_EQ(sender, label_checkbox_);
  label_->SetText(label_checkbox_->checked() ? textfield_->text() :
                                               base::string16());
  container()->Layout();
  container()->SchedulePaint();
}

}  // namespace examples
}  // namespace views
