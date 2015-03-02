// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "azer/ui/views/bubble/bubble_window_targeter.h"

#include "azer/ui/aura/window.h"
#include "ui/gfx/path.h"
#include "ui/gfx/skia_util.h"
#include "azer/ui/views/bubble/bubble_delegate.h"
#include "azer/ui/views/bubble/bubble_frame_view.h"

namespace views {

BubbleWindowTargeter::BubbleWindowTargeter(BubbleDelegateView* bubble)
    : wm::MaskedWindowTargeter(bubble->GetWidget()->GetNativeView()),
      bubble_(bubble) {
}

BubbleWindowTargeter::~BubbleWindowTargeter() {
}

bool BubbleWindowTargeter::GetHitTestMask(aura::Window* window,
                                          gfx::Path* mask) const {
  mask->addRect(
      gfx::RectToSkRect(bubble_->GetBubbleFrameView()->GetContentsBounds()));
  return true;
}

}  // namespace views
