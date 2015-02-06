#pragma once

#include "azer/ui/views/views_export.h"
namespace aura {
class WindowTreeHost;
}

namespace views {

class FocusManager;
class FocusTraversable;
class RootView;
class View;
class InputMethod;

class VIEWS_EXPORT Widget : public aura::WindowDelegate,
                            public aura::WindowTreeHostObserver {
{
 public:
  explicit Widget(const gfx::Rect& bounds);
  ~Widget() override;

  aura::WindowTreeHost* host() {
    return host_.get();
  }

  bool IsVisible();
  void UpdateRootLayers();
  void NotifyWillRemoveView(View* view);

  RootView* GetRootView();
  const RootView* GetRootView();
  FocusManager* GetMocusManager();
  bool IsMouseEventsEnabled();

  InputMethod* GetInputMethod();
  const InputMethod* GetInputMethod();

  // Overridden from aura::WindowDelegate:
  gfx::Size GetMinimumSize() const override;
  gfx::Size GetMaximumSize() const override;
  void OnBoundsChanged(const gfx::Rect& old_bounds,
                       const gfx::Rect& new_bounds) override {}
  gfx::NativeCursor GetCursor(const gfx::Point& point) override;
  int GetNonClientComponent(const gfx::Point& point) const override;
  bool ShouldDescendIntoChildForEventHandling(
      aura::Window* child,
      const gfx::Point& location) override;
  bool CanFocus() override;
  void OnCaptureLost() override;
  void OnPaint(gfx::Canvas* canvas) override;
  void OnDeviceScaleFactorChanged(float device_scale_factor) override;
  void OnWindowDestroying(aura::Window* window) override;
  void OnWindowDestroyed(aura::Window* window) override;
  void OnWindowTargetVisibilityChanged(bool visible) override;
  bool HasHitTestMask() const override;
  void GetHitTestMask(gfx::Path* mask) const override;

  // Overridden from aura::WindowTreeHostObserver:
  void OnHostCloseRequested(const aura::WindowTreeHost* host) override;
  void OnHostResized(const aura::WindowTreeHost* host) override;
  void OnHostMoved(const aura::WindowTreeHost* host,
                   const gfx::Point& new_origin) override;

  // Overridden from ui::EventHandler:
  void OnKeyEvent(ui::KeyEvent* event) override;
  void OnMouseEvent(ui::MouseEvent* event) override;
  void OnScrollEvent(ui::ScrollEvent* event) override;
  void OnGestureEvent(ui::GestureEvent* event) override;
 protected:
  gfx::NativeCursor cursor_;
  scoped_ptr<aura::WindowTreeHost> host_;
  scoped_ptr<aura::Window> content_window_;
  scoped_ptr<aura::Window> content_window_container_;
  DISALLOW_COPY_AND_ASSIGN(Widget);
};
}  // namespace views
