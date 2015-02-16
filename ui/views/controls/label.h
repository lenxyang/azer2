#pragma once

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/font_list.h"
#include "ui/gfx/shadow_value.h"
#include "ui/gfx/text_constants.h"

#include "azer/ui/views/controls/control.h"

namespace views {
class VIEWS_EXPORT Label : public Control {
 public:
  struct VIEWS_EXPORT Params {
    int32 line_height;
    bool multi_line;
    bool obscured;
    SkColor enabled_color;
    SkColor disabled_color;
    SkColor background_color;
    gfx::ElideBehavior elide_behavior;
    gfx::HorizontalAlignment horizontal_alignment;
    gfx::FontList fontlist;
    gfx::ShadowValues shadows;

    Params();
  };

  Label();
  explicit Label(const base::string16& text);
  Label(const base::string16& text, const gfx::FontList& font_list);
  ~Label() override;

  // Gets or sets the fonts used by this label.
  const gfx::FontList& font_list() const { return font_list_; }
  virtual void SetFontList(const gfx::FontList& font_list);

  // Get or set the label text.
  const base::string16& text() const { return text_; }
  virtual void SetText(const base::string16& text);

  const Params& params() const { return params_;}
  void SetParam(const Params& param);
 protected:
  void SetTextInternal(const base::string16& text);
  // The cached heights to avoid recalculation in GetHeightForWidth().
  Params params_;
  DISALLOW_COPY_AND_ASSIGN(Label);
};
}  // namespace views
