// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SCROLLBAR_NATIVE_SCROLL_BAR_VIEWS_H_
#define UI_VIEWS_CONTROLS_SCROLLBAR_NATIVE_SCROLL_BAR_VIEWS_H_

#include "base/compiler_specific.h"
#include "ui/gfx/point.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/controls/scrollbar/base_scroll_bar.h"
#include "ui/views/controls/scrollbar/native_scroll_bar_wrapper.h"
#include "ui/views/view.h"

namespace gfx {
class Canvas;
}

namespace views {

class NativeScrollBar;

// Views implementation for the scrollbar.
class VIEWS_EXPORT NativeScrollBarViews : public BaseScrollBar,
                                          public ButtonListener,
                                          public NativeScrollBarWrapper {
 public:
  static const char kViewClassName[];

  // Creates new scrollbar, either horizontal or vertical.
  explicit NativeScrollBarViews(NativeScrollBar* native_scroll_bar);
  ~NativeScrollBarViews() override;

 private:
  // View overrides:
  void Layout() override;
  void OnPaint(gfx::Canvas* canvas) override;
  gfx::Size GetPreferredSize() const override;
  const char* GetClassName() const override;

  // ScrollBar overrides:
  int GetLayoutSize() const override;

  // BaseScrollBar overrides:
  void ScrollToPosition(int position) override;
  int GetScrollIncrement(bool is_page, bool is_positive) override;

  // BaseButton::ButtonListener overrides:
  void ButtonPressed(Button* sender, const ui::Event& event) override;

  // NativeScrollBarWrapper overrides:
  int GetPosition() const override;
  View* GetView() override;
  void Update(int viewport_size, int content_size, int current_pos) override;

  // Returns the area for the track. This is the area of the scrollbar minus
  // the size of the arrow buttons.
  gfx::Rect GetTrackBounds() const override;

  // The NativeScrollBar we are bound to.
  NativeScrollBar* native_scroll_bar_;

  // The scroll bar buttons (Up/Down, Left/Right).
  Button* prev_button_;
  Button* next_button_;

  ui::NativeTheme::ExtraParams params_;
  ui::NativeTheme::Part part_;
  ui::NativeTheme::State state_;

  DISALLOW_COPY_AND_ASSIGN(NativeScrollBarViews);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_SCROLLBAR_NATIVE_SCROLL_BAR_VIEWS_H_
