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
#include "azer/ui/views/view.h"

namespace ui {
class Accelerator;
class DefaultThemeProvider;
class InputMethod;
class NativeTheme;
class ThemeProvider;
}

namespace aura {
class Window;
class WindowTreeHost;

namespace client {
class FocusClient;
class EventClient;
}  // namespace client
}  // namespace aura

namespace views {
class VIEWS_EXPORT RootView : public aura::WindowTreeHostObserver,
                              public ui::NativeThemeObserver,
                              public View {
 public:
  RootView();
  virtual ~RootView();

  struct VIEWS_EXPORT InitParams {
    gfx::Rect bounds;
  };
  void Init(const InitParams& param);
  void Close();
  bool IsClosing() const { return closing_;}

  void Show() override;
  void Hide() override;

  // Returns the ThemeProvider that provides theme resources for this Widget.
  virtual ui::ThemeProvider* GetThemeProvider() const;

  // Overridden from ui::NativeThemeObserver:
  void OnNativeThemeUpdated(ui::NativeTheme* observed_theme) override;

  ui::NativeTheme* GetNativeTheme() {
    return const_cast<ui::NativeTheme*>(
        const_cast<const RootView*>(this)->GetNativeTheme());
  }
  const ui::NativeTheme* GetNativeTheme() const;

  aura::WindowTreeHost* host() { return host_.get();}
  const aura::WindowTreeHost* host() const { return host_.get();}
 private:
  scoped_ptr<aura::client::FocusClient> focus_client_;
  scoped_ptr<aura::client::EventClient> event_client_;
  scoped_ptr<aura::WindowTreeHost> host_;
  bool closing_;

  // A theme provider to use when no other theme provider is specified.
  scoped_ptr<ui::DefaultThemeProvider> default_theme_provider_;

  ScopedObserver<ui::NativeTheme, ui::NativeThemeObserver> observer_manager_;
  DISALLOW_COPY_AND_ASSIGN(RootView);
};
}  // namespace views
