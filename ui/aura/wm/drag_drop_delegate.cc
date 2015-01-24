// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "azer/ui/aura/wm/drag_drop_delegate.h"

#include "azer/ui/aura/window.h"
#include "azer/ui/aura/window_property.h"

DECLARE_WINDOW_PROPERTY_TYPE(aura::client::DragDropDelegate*)

namespace aura {
namespace client {

DEFINE_LOCAL_WINDOW_PROPERTY_KEY(
    DragDropDelegate*, kDragDropDelegateKey, NULL);

void SetDragDropDelegate(Window* window, DragDropDelegate* delegate) {
  window->SetProperty(kDragDropDelegateKey, delegate);
}

DragDropDelegate* GetDragDropDelegate(Window* window) {
  return window->GetProperty(kDragDropDelegateKey);
}

}  // namespace client
}  // namespace aura
