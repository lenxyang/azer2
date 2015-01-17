#pragma once

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "ui/gfx/font_list.h"
#include "ui/gfx/shadow_value.h"
#include "ui/gfx/text_constants.h"
#include "azer/ui/views/view.h"

namespace azer {
namespace views {

class AZER_EXPORT Label : public View {
 public:
  Label();
  explicit Label(const ::base::string16& text);
  Label(const ::base::string16& text, const gfx::FontList& font_list);
  ~Label() override;

  // Gets or sets the fonts used by this label.
  const gfx::FontList& font_list() const { return font_list_; }
  virtual void SetFontList(const gfx::FontList& font_list);

  // Get or set the label text.
  const base::string16& text() const { return text_; }
  virtual void SetText(const base::string16& text);
  
  // Get or set the distance in pixels between baselines of multi-line text.
  // Default is 0, indicating the distance between lines should be the standard
  // one for the label's text, font list, and platform.
  int line_height() const { return line_height_; }
  void SetLineHeight(int height);

  // Get or set if the label text can wrap on multiple lines; default is false.
  bool multi_line() const { return multi_line_; }
  void SetMultiLine(bool multi_line);

  // Get or set if the label text should be obscured before rendering (e.g.
  // should "Password!" display as "*********"); default is false.
  bool obscured() const { return obscured_; }
  void SetObscured(bool obscured);
 private:
  base::string16 text_;
  gfx::FontList font_list_;
  int line_height_;
  bool multi_line_;
  bool obscured_;
  DISALLOW_COPY_AND_ASSIGN(Label);
};
}  // namespace views
}  // namespace azer
