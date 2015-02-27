#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/scoped_observer.h"
#include "ui/base/cursor/cursor.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/native_theme/native_theme_observer.h"

#include "azer/ui/aura/window_delegate.h"
#include "azer/ui/aura/window_tree_host_observer.h"
#include "azer/ui/views/views_export.h"

namespace ui {
class DefaultThemeProvider;
class ThemeProvider;
}  // namespace ui

namespace aura  {
class WindowTreeHost;

namespace client {
class FocusClient;
}  // namespace client
}  // namespace aura

namespace views {

class View;
class EventClient;
namespace internal {
class RootView;
}  // namespace internal

class VIEWS_EXPORT Widget : public aura::WindowTreeHostObserver,
                            public ui::NativeThemeObserver {
 public:
  Widget();
  virtual ~Widget();

  struct VIEWS_EXPORT InitParams {
    gfx::Rect bounds;
  };
  void Init(const InitParams& param);
  void Close();
  void CloseNow();
  bool IsClosing() const { return closing_;}

  void Show();
  void Hide();

  bool IsVisible() const;

  // Sets the specified view as the contents of this Widget. There can only
  // be one contents view child of this Widget's RootView. This view is sized to
  // fit the entire size of the RootView. The RootView takes ownership of this
  // View, unless it is set as not being parent-owned.
  void SetContentsView(View* view);
  View* GetContentsView();

  // Returns the bounds of the Widget in screen coordinates.
  gfx::Rect GetWindowBoundsInScreen() const;

  // Returns the bounds of the Widget's client area in screen coordinates.
  gfx::Rect GetClientAreaBoundsInScreen() const;

  // Sizes and/or places the widget to the specified bounds, size or position.
  void SetBounds(const gfx::Rect& bounds);
  void SetSize(const gfx::Size& size);

  // Sizes the window to the specified size and centerizes it.
  void CenterWindow(const gfx::Size& size);

  // Returns the ThemeProvider that provides theme resources for this Widget.
  virtual ui::ThemeProvider* GetThemeProvider() const;

  ui::NativeTheme* GetNativeTheme() {
    return const_cast<ui::NativeTheme*>(
        const_cast<const Widget*>(this)->GetNativeTheme());
  }
  const ui::NativeTheme* GetNativeTheme() const;

  internal::RootView* GetRootView() { return root_view_.get();}
  const internal::RootView* GetRootView() const { return root_view_.get();}

  aura::WindowTreeHost* host() { return host_.get();}
  const aura::WindowTreeHost* host() const { return host_.get();}

  // internal
  void OnRootViewSetBoundsChanged(const gfx::Rect& bounds);
 protected:
  void OnNativeThemeUpdated(ui::NativeTheme* observed_theme) override;
  scoped_ptr<aura::WindowTreeHost> host_;
  scoped_ptr<internal::RootView> root_view_;
  scoped_ptr<aura::client::FocusClient> focus_client_;
  scoped_ptr<EventClient> event_client_;
  bool closing_;

  // A theme provider to use when no other theme provider is specified.
  scoped_ptr<ui::DefaultThemeProvider> default_theme_provider_;

  ScopedObserver<ui::NativeTheme, ui::NativeThemeObserver> observer_manager_;
  DISALLOW_COPY_AND_ASSIGN(Widget);
};
}  // namespace views
