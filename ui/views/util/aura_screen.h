// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_UTIL_AURA_SCREEN_H_
#define UI_VIEWS_UTIL_AURA_SCREEN_H_

#include "base/compiler_specific.h"
#include "azer/ui/aura/window_observer.h"
#include "ui/gfx/display.h"
#include "ui/gfx/screen.h"
#include "azer/ui/views/views_export.h"

namespace gfx {
class Insets;
class Rect;
class Transform;
}

namespace aura {
class Window;
class WindowTreeHost;
}  // namespace aura

namespace views {

// A minimal, testing Aura implementation of gfx::Screen.
class VIEWS_EXPORT AuraScreen : public gfx::Screen,
                                public aura::WindowObserver {
 public:
  // Creates a gfx::Screen of the specified size. If no size is specified, then
  // creates a 800x600 screen. |size| is in physical pixels.
  static AuraScreen* Create(const gfx::Size& size);
  // Creates a AuraScreen that uses fullscreen for the display.
  static AuraScreen* CreateFullscreen();
  ~AuraScreen() override;

  aura::WindowTreeHost* CreateHostForPrimaryDisplay();

  void SetDeviceScaleFactor(float device_scale_fator);
  void SetDisplayRotation(gfx::Display::Rotation rotation);
  void SetUIScale(float ui_scale);
  void SetWorkAreaInsets(const gfx::Insets& insets);

 protected:
  gfx::Transform GetRotationTransform() const;
  gfx::Transform GetUIScaleTransform() const;

  // WindowObserver overrides:
  void OnWindowBoundsChanged(aura::Window* window,
                             const gfx::Rect& old_bounds,
                             const gfx::Rect& new_bounds) override;
  void OnWindowDestroying(aura::Window* window) override;

  // gfx::Screen overrides:
  gfx::Point GetCursorScreenPoint() override;
  gfx::NativeWindow GetWindowUnderCursor() override;
  gfx::NativeWindow GetWindowAtScreenPoint(const gfx::Point& point) override;
  int GetNumDisplays() const override;
  std::vector<gfx::Display> GetAllDisplays() const override;
  gfx::Display GetDisplayNearestWindow(gfx::NativeView view) const override;
  gfx::Display GetDisplayNearestPoint(const gfx::Point& point) const override;
  gfx::Display GetDisplayMatching(const gfx::Rect& match_rect) const override;
  gfx::Display GetPrimaryDisplay() const override;
  void AddObserver(gfx::DisplayObserver* observer) override;
  void RemoveObserver(gfx::DisplayObserver* observer) override;

 private:
  explicit AuraScreen(const gfx::Rect& screen_bounds);

  aura::WindowTreeHost* host_;

  gfx::Display display_;

  float ui_scale_;

  DISALLOW_COPY_AND_ASSIGN(AuraScreen);
};

}  // namespace views

#endif  // UI_VIEWS_UTIL_AURA_SCREEN_H_
