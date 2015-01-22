// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "azer/ui/aura/client/window_stacking_client.h"

namespace aura {
namespace client {

namespace {

WindowStackingClient* instance = NULL;

}  // namespace

void SetWindowStackingClient(WindowStackingClient* client) {
  instance = client;
}

WindowStackingClient* GetWindowStackingClient() {
  return instance;
}

}  // namespace client
}  // namespace aura