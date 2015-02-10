#pragma once

#include "base/memory/scoped_ptr.h"
#include "ui/base/cursor/cursor.h"

#include "azer/ui/views/views_export.h"
#include "azer/ui/views/widget/root_view.h"
#include "azer/ui/aura/window_delegate.h"
#include "azer/ui/aura/window_tree_host_observer.h"

namespace aura {
class WindowTreeHost;
namespace client {
class FocusClient;
}  // namespace client
}

namespace views {

class FocusManager;
class FocusTraversable;
class RootView;
class View;
class InputMethod;

class VIEWS_EXPORT Widget : public aura::WindowDelegate,
                            public aura::WindowTreeHostObserver {
 public:
  struct VIEWS_EXPORT InitParams {
    gfx::Rect bounds;
  };

  explicit Widget();
  ~Widget() override;

  void Init(const InitParams& params);

  aura::WindowTreeHost* host() {
    return host_.get();
  }

  // Sets a shape on the widget. Passing a NULL |shape| reverts the widget to
  // be rectangular. Takes ownership of |shape|.
  void SetShape(gfx::NativeRegion shape);

  // Hides the widget then closes it after a return to the message loop.
  virtual void Close();

  // Whether the widget has been asked to close itself. In particular this is
  // set to true after Close() has been invoked on the NativeWidget.
  bool IsClosed() const;

  // Shows the widget. The widget is activated if during initialization the
  // can_activate flag in the InitParams structure is set to true.
  virtual void Show();
  // Hides the widget.
  void Hide();

  // Like Show(), but does not activate the window.
  void ShowInactive();

  // Activates the widget, assuming it already exists and is visible.
  void Activate();

  // Deactivates the widget, making the next window in the Z order the active
  // window.
  void Deactivate();

  // Returns whether the Widget is the currently active window.
  virtual bool IsActive() const;

  bool IsVisible();
  void UpdateRootLayers();
  void NotifyWillRemoveView(View* view);

  RootView* GetRootView();
  const RootView* GetRootView() const;

  bool IsMouseEventsEnabled();
  InputMethod* GetInputMethod();
  const InputMethod* GetInputMethod() const;

  virtual bool CanActivate() const;

  // Overridden from aura::WindowDelegate:
  gfx::Size GetMinimumSize() const override;
  gfx::Size GetMaximumSize() const override;
  void OnBoundsChanged(const gfx::Rect& old_bounds,
                       const gfx::Rect& new_bounds) override;
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
  scoped_ptr<RootView> root_view_;
  scoped_ptr<aura::WindowTreeHost> host_;
  scoped_ptr<aura::client::FocusClient> focus_client_;
  aura::Window* content_window_;
  scoped_ptr<aura::Window> content_window_container_;

  bool widget_closed_;
  DISALLOW_COPY_AND_ASSIGN(Widget);
};
}  // namespace views
