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
    // Internal class name.
  static const char kViewClassName[];

  // The padding for the focus border when rendering focused text.
  static const int kFocusBorderPadding;

  Label(View* parent);
  Label(const ::base::string16& text, View* parent);
  Label(const ::base::string16& text, const gfx::FontList& font_list, View* parent);
  ~Label() override;

  // Gets or sets the fonts used by this label.
  const gfx::FontList& font_list() const { return font_list_; }
  virtual void SetFontList(const gfx::FontList& font_list);

  // Get or set the label text.
  const base::string16& text() const { return text_; }
  virtual void SetText(const base::string16& text);

    // Sets the color.  This will automatically force the color to be readable
  // over the current background color, if auto color readability is enabled.
  virtual void SetEnabledColor(SkColor color);
  void SetDisabledColor(SkColor color);

  SkColor enabled_color() const { return actual_enabled_color_; }

  // Sets the background color.  This won't be explicitly drawn, but the label
  // will force the text color to be readable over it.
  void SetBackgroundColor(SkColor color);
  SkColor background_color() const { return background_color_; }

  // Set drop shadows underneath the text.
  void SetShadows(const gfx::ShadowValues& shadows);
  const gfx::ShadowValues& shadows() const { return shadows_; }

  // Sets whether subpixel rendering is used; the default is true, but this
  // feature also requires an opaque background color.
  void SetSubpixelRenderingEnabled(bool subpixel_rendering_enabled);

  // Sets the horizontal alignment; the argument value is mirrored in RTL UI.
  void SetHorizontalAlignment(gfx::HorizontalAlignment alignment);
  gfx::HorizontalAlignment GetHorizontalAlignment() const;
  
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
 protected:
  // Called by Paint to paint the text.
  void PaintText(gfx::Canvas* canvas,
                 const base::string16& text,
                 const gfx::Rect& text_bounds,
                 int flags);

  virtual gfx::Size GetTextSize() const;
  void OnPaint(gfx::Canvas* canvas) override;

    // Sets both |text_| and |layout_text_| to appropriate values, taking
  // the label's 'obscured' status into account.
  void SetTextInternal(const base::string16& text);

  void Init(const base::string16& text, const gfx::FontList& font_list);
 private:
  void RecalculateColors();
  // Returns where the text is drawn, in the receivers coordinate system.
  gfx::Rect GetTextBounds() const;

  int ComputeDrawStringFlags() const;

  gfx::Rect GetAvailableRect() const;

  // Returns parameters to be used for the DrawString call.
  void CalculateDrawStringParams(base::string16* paint_text,
                                 gfx::Rect* text_bounds,
                                 int* flags) const;

  // Resets |cached_heights_| and |cached_heights_cursor_| and mark
  // |text_size_valid_| as false.
  void ResetCachedSize();

  base::string16 text_;
  base::string16 layout_text_;
  gfx::FontList font_list_;
  
  SkColor requested_enabled_color_;
  SkColor actual_enabled_color_;
  SkColor requested_disabled_color_;
  SkColor actual_disabled_color_;
  SkColor background_color_;

  // Set to true once the corresponding setter is invoked.
  bool enabled_color_set_;
  bool disabled_color_set_;
  bool background_color_set_;

  bool subpixel_rendering_enabled_;
  bool auto_color_readability_;
  mutable gfx::Size text_size_;
  mutable bool text_size_valid_;
  int line_height_;
  bool multi_line_;
  bool obscured_;
  bool allow_character_break_;
  bool collapse_when_hidden_;

  gfx::ElideBehavior elide_behavior_;
  gfx::HorizontalAlignment horizontal_alignment_;
  gfx::ShadowValues shadows_;

  // The cached heights to avoid recalculation in GetHeightForWidth().
  mutable std::vector<gfx::Size> cached_heights_;
  DISALLOW_COPY_AND_ASSIGN(Label);
};
}  // namespace views
}  // namespace azer
