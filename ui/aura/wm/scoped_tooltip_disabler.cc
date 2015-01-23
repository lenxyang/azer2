// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "azer/azer/ui/aura/wm/scoped_tooltip_disabler.h"

#include "azer/ui/aura/window.h"
#include "azer/azer/ui/aura/wm/tooltip_client.h"

namespace aura {
namespace client {

ScopedTooltipDisabler::ScopedTooltipDisabler(aura::Window* window)
    : root_(window ? window->GetRootWindow() : NULL) {
  if (root_) {
    root_->AddObserver(this);
    TooltipClient* client = GetTooltipClient(root_);
    if (client)
      client->SetTooltipsEnabled(false);
  }
}

ScopedTooltipDisabler::~ScopedTooltipDisabler() {
  EnableTooltips();
}

void ScopedTooltipDisabler::EnableTooltips() {
  if (!root_)
    return;
  TooltipClient* client = GetTooltipClient(root_);
  if (client)
    client->SetTooltipsEnabled(true);
  root_->RemoveObserver(this);
  root_ = NULL;
}

void ScopedTooltipDisabler::OnWindowDestroying(aura::Window* window) {
  EnableTooltips();
}


}  // namespace client
}  // namespace aura
