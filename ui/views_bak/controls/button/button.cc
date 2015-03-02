// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "azer/ui/views/controls/button/button.h"

#include "base/strings/utf_string_conversions.h"
#include "ui/accessibility/ax_view_state.h"

namespace views {

////////////////////////////////////////////////////////////////////////////////
// Button, static public:

// static
Button::ButtonState Button::GetButtonStateFrom(ui::NativeTheme::State state) {
  switch (state) {
    case ui::NativeTheme::kDisabled:  return Button::STATE_DISABLED;
    case ui::NativeTheme::kHovered:   return Button::STATE_HOVERED;
    case ui::NativeTheme::kNormal:    return Button::STATE_NORMAL;
    case ui::NativeTheme::kPressed:   return Button::STATE_PRESSED;
    case ui::NativeTheme::kNumStates: NOTREACHED();
  }
  return Button::STATE_NORMAL;
}

////////////////////////////////////////////////////////////////////////////////
// Button, public:

Button::~Button() {
}

////////////////////////////////////////////////////////////////////////////////
// Button, View overrides:

////////////////////////////////////////////////////////////////////////////////
// Button, protected:

Button::Button(ButtonListener* listener)
    : listener_(listener),
      tag_(-1) {
  // SetAccessibilityFocusable(true);
}

void Button::NotifyClick(const ui::Event& event) {
  // We can be called when there is no listener, in cases like double clicks on
  // menu buttons etc.
  if (listener_)
    listener_->ButtonPressed(this, event);
}

void Button::SetAccessibleName(const base::string16& name) {
}
}  // namespace views
