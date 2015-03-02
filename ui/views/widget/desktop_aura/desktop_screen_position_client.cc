// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "azer/ui/views/widget/desktop_aura/desktop_screen_position_client.h"

#include "azer/ui/views/widget/desktop_aura/desktop_native_widget_aura.h"

namespace views {

namespace {

// Returns true if bounds passed to window in SetBounds should be treated as
// though they are in screen coordinates.
bool PositionWindowInScreenCoordinates(aura::Window* window) {
  if (window->type() == ui::wm::WINDOW_TYPE_POPUP)
    return true;

  Widget* widget = Widget::GetWidgetForNativeView(window);
  return widget && widget->is_top_level();
}

}  // namespace

DesktopScreenPositionClient::DesktopScreenPositionClient(
    aura::Window* root_window)
    : wm::DefaultScreenPositionClient(), root_window_(root_window) {
  aura::client::SetScreenPositionClient(root_window_, this);
}

DesktopScreenPositionClient::~DesktopScreenPositionClient() {
  aura::client::SetScreenPositionClient(root_window_, NULL);
}

void DesktopScreenPositionClient::SetBounds(aura::Window* window,
                                            const gfx::Rect& bounds,
                                            const gfx::Display& display) {
  // TODO(jam): Use the 3rd parameter, |display|.
  aura::Window* root = window->GetRootWindow();

  // This method assumes that |window| does not have an associated
  // DesktopNativeWidgetAura.
  internal::NativeWidgetPrivate* desktop_native_widget =
      DesktopNativeWidgetAura::ForWindow(root);
  DCHECK(!desktop_native_widget ||
         desktop_native_widget->GetNativeView() != window);

  if (PositionWindowInScreenCoordinates(window)) {
    // The caller expects windows we consider "embedded" to be placed in the
    // screen coordinate system. So we need to offset the root window's
    // position (which is in screen coordinates) from these bounds.

    gfx::Point origin = bounds.origin();
    aura::Window::ConvertPointToTarget(window->parent(), root, &origin);

    gfx::Point host_origin = GetOriginInScreen(root);
    origin.Offset(-host_origin.x(), -host_origin.y());
    window->SetBounds(gfx::Rect(origin, bounds.size()));
    return;
  }

  window->SetBounds(bounds);
}

}  // namespace views
