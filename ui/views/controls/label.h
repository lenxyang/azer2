// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_LABEL_H_
#define UI_VIEWS_CONTROLS_LABEL_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/font_list.h"
#include "ui/gfx/shadow_value.h"
#include "ui/gfx/text_constants.h"
#include "azer/ui/views/view.h"

namespace views {

// A view subclass that can display a string.
class VIEWS_EXPORT Label : public View {
 public:
  // Internal class name.
  static const char kViewClassName[];

  // The padding for the focus border when rendering focused text.
  static const int kFocusBorderPadding;

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

  // Enables or disables auto-color-readability (enabled by default).  If this
  // is enabled, then calls to set any foreground or background color will
  // trigger an automatic mapper that uses color_utils::GetReadableColor() to
  // ensure that the foreground colors are readable over the background color.
  void SetAutoColorReadabilityEnabled(bool enabled);

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

  // Sets whether multi-line text can wrap mid-word; the default is false.
  void SetAllowCharacterBreak(bool allow_character_break);

  // Sets the eliding or fading behavior, applied as necessary. The default is
  // to elide at the end. Eliding is not well supported for multi-line labels.
  void SetElideBehavior(gfx::ElideBehavior elide_behavior);

  // Sets the tooltip text.  Default behavior for a label (single-line) is to
  // show the full text if it is wider than its bounds.  Calling this overrides
  // the default behavior and lets you set a custom tooltip.  To revert to
  // default behavior, call this with an empty string.
  void SetTooltipText(const base::string16& tooltip_text);

  // Get or set whether this label can act as a tooltip handler; the default is
  // true.  Set to false whenever an ancestor view should handle tooltips
  // instead.
  bool handles_tooltips() const { return handles_tooltips_; }
  void SetHandlesTooltips(bool enabled);

  // Resizes the label so its width is set to the width of the longest line and
  // its height deduced accordingly.
  // This is only intended for multi-line labels and is useful when the label's
  // text contains several lines separated with \n.
  // |max_width| is the maximum width that will be used (longer lines will be
  // wrapped).  If 0, no maximum width is enforced.
  void SizeToFit(int max_width);

  // Sets whether the preferred size is empty when the label is not visible.
  void set_collapse_when_hidden(bool value) { collapse_when_hidden_ = value; }

  // Get the text as displayed to the user, respecting the obscured flag.
  const base::string16& GetLayoutTextForTesting() const;

  // View:
  gfx::Insets GetInsets() const override;
  int GetBaseline() const override;
  gfx::Size GetPreferredSize() const override;
  gfx::Size GetMinimumSize() const override;
  int GetHeightForWidth(int w) const override;
  const char* GetClassName() const override;
  View* GetTooltipHandlerForPoint(const gfx::Point& point) override;
  bool CanProcessEventsWithinSubtree() const override;
  void GetAccessibleState(ui::AXViewState* state) override;
  bool GetTooltipText(const gfx::Point& p,
                      base::string16* tooltip) const override;

 protected:
  // Called by Paint to paint the text.
  void PaintText(gfx::Canvas* canvas,
                 const base::string16& text,
                 const gfx::Rect& text_bounds,
                 int flags);

  virtual gfx::Size GetTextSize() const;

  SkColor disabled_color() const { return actual_disabled_color_; }

  // View:
  void OnBoundsChanged(const gfx::Rect& previous_bounds) override;
  void OnPaint(gfx::Canvas* canvas) override;
 private:
  // These tests call CalculateDrawStringParams in order to verify the
  // calculations done for drawing text.
  FRIEND_TEST_ALL_PREFIXES(LabelTest, DrawSingleLineString);
  FRIEND_TEST_ALL_PREFIXES(LabelTest, DrawMultiLineString);
  FRIEND_TEST_ALL_PREFIXES(LabelTest, DrawSingleLineStringInRTL);
  FRIEND_TEST_ALL_PREFIXES(LabelTest, DrawMultiLineStringInRTL);
  FRIEND_TEST_ALL_PREFIXES(LabelTest, DirectionalityFromText);
  FRIEND_TEST_ALL_PREFIXES(LabelTest, DisableSubpixelRendering);

  // Sets both |text_| and |layout_text_| to appropriate values, taking
  // the label's 'obscured' status into account.
  void SetTextInternal(const base::string16& text);

  void Init(const base::string16& text, const gfx::FontList& font_list);

  void RecalculateColors();

  // Returns where the text is drawn, in the receivers coordinate system.
  gfx::Rect GetTextBounds() const;

  int ComputeDrawStringFlags() const;

  gfx::Rect GetAvailableRect() const;

  // Returns parameters to be used for the DrawString call.
  void CalculateDrawStringParams(base::string16* paint_text,
                                 gfx::Rect* text_bounds,
                                 int* flags) const;

  // Updates any colors that have not been explicitly set from the theme.
  void UpdateColorsFromTheme(const ui::NativeTheme* theme);

  // Resets |cached_heights_| and |cached_heights_cursor_| and mark
  // |text_size_valid_| as false.
  void ResetCachedSize();

  bool ShouldShowDefaultTooltip() const;

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
  gfx::ElideBehavior elide_behavior_;
  gfx::HorizontalAlignment horizontal_alignment_;
  base::string16 tooltip_text_;
  bool handles_tooltips_;
  // Whether to collapse the label when it's not visible.
  bool collapse_when_hidden_;
  gfx::ShadowValues shadows_;

  // The cached heights to avoid recalculation in GetHeightForWidth().
  mutable std::vector<gfx::Size> cached_heights_;
  mutable int cached_heights_cursor_;

  DISALLOW_COPY_AND_ASSIGN(Label);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_LABEL_H_
