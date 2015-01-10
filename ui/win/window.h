#pragma once

#include "base/basictypes.h"
#include "azer/base/export.h"

namespace azer {
namespace win {

class WindowTreeHost;
class WindowDelegate;

class AZER_EXPORT Window {
 public:
  Window();
  virtual ~Window();

  WindowDelegate* delegate() { return delegate_; }
  const WindowDelegate* delegate() const { return delegate_; }

  const gfx::Rect& bounds() const { return bounds_; }

  Window* parent() { return parent_; }
  const Window* parent() const { return parent_; }

  // Returns the root Window that contains this Window. The root Window is
  // defined as the Window that has a dispatcher. These functions return NULL if
  // the Window is contained in a hierarchy that does not have a dispatcher at
  // its root.
  Window* GetRootWindow();
  const Window* GetRootWindow() const;

  WindowTreeHost* GetHost();
  const WindowTreeHost* GetHost() const;
  void set_host(WindowTreeHost* host) { host_ = host; }
  bool IsRootWindow() const { return !!host_; }

  // The Window does not own this object.
  void set_user_data(void* user_data) { user_data_ = user_data; }
  void* user_data() const { return user_data_; }

    // Changes the visibility of the window.
  void Show();
  void Hide();
  // Returns true if this window and all its ancestors are visible.
  bool IsVisible() const;
  // Returns the visibility requested by this window. IsVisible() takes into
  // account the visibility of the layer and ancestors, where as this tracks
  // whether Show() without a Hide() has been invoked.
  bool TargetVisibility() const { return visible_; }

  // Returns the window's bounds in root window's coordinates.
  gfx::Rect GetBoundsInRootWindow() const;

  // Returns the window's bounds in screen coordinates.
  // How the root window's coordinates is mapped to screen's coordinates
  // is platform dependent and defined in the implementation of the
  // |aura::client::ScreenPositionClient| interface.
  gfx::Rect GetBoundsInScreen() const;

  void SetTransform(const gfx::Transform& transform);

  // Changes the bounds of the window. If present, the window's parent's
  // LayoutManager may adjust the bounds.
  void SetBounds(const gfx::Rect& new_bounds);
 private:
  Window* parent_;
  WindowTreeHost* host_;
  WindowDelegate* delegate_;
  bool visible_;
  void *user_data_;
  gfx::Rect bounds_;
  DISALLOW_COPY_AND_ASSIGN(Window);
};
}  // namespace win
}  // namespace azer
