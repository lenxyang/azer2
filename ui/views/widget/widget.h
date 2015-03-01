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
#include "azer/ui/views/ime/input_method_delegate.h"

namespace ui {
class Accelerator;
class DefaultThemeProvider;
class InputMethod;
class NativeTheme;
class OSExchangeData;
class ThemeProvider;
}  // namespace ui

namespace aura  {
class WindowTreeHost;

namespace client {
class FocusClient;
}  // namespace client
}  // namespace aura

namespace views {

class EventClient;
class InputMethod;
class View;

namespace internal {
class RootView;
}  // namespace internal

class VIEWS_EXPORT Widget : public aura::WindowTreeHostObserver,
                            public internal::InputMethodDelegate,
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

  // Sets the widget to be on top of all other widgets in the windowing system.
  void SetAlwaysOnTop(bool on_top);

  // Returns whether the widget has been set to be on top of most other widgets
  // in the windowing system.
  bool IsAlwaysOnTop() const;

  // Sets the widget to be visible on all work spaces.
  void SetVisibleOnAllWorkspaces(bool always_visible);

  // Maximizes/minimizes/restores the window.
  void Maximize();
  void Minimize();
  void Restore();

  // Whether or not the window is maximized or minimized.
  virtual bool IsMaximized() const;
  bool IsMinimized() const;

  // Accessors for fullscreen state.
  void SetFullscreen(bool fullscreen);
  bool IsFullscreen() const;

  // Returns the View at the root of the View hierarchy contained by this
  // Widget.
  internal::RootView* GetRootView() { return root_view_.get();}
  const internal::RootView* GetRootView() const { return root_view_.get();}


  bool IsVisible() const;

  // Returns the top level widget in a hierarchy (see is_top_level() for
  // the definition of top level widget.) Will return NULL if called
  // before the widget is attached to the top level widget's hierarchy.
  Widget* GetTopLevelWidget();
  const Widget* GetTopLevelWidget() const;

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

  // Returns the work area bounds of the screen the Widget belongs to.
  gfx::Rect GetWorkAreaBoundsInScreen() const;

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

  // Returns the FocusManager for this widget.
  // Note that all widgets in a widget hierarchy share the same focus manager.
  FocusManager* GetFocusManager();
  const FocusManager* GetFocusManager() const;

  // Returns the InputMethod for this widget.
  // Note that all widgets in a widget hierarchy share the same input method.
  InputMethod* GetInputMethod();
  const InputMethod* GetInputMethod() const;

  // Returns the ui::InputMethod for this widget.
  // TODO(yukishiino): Rename this method to GetInputMethod once we remove
  // views::InputMethod.
  ui::InputMethod* GetHostInputMethod();

  // Retrieves the focus traversable for this widget.
  FocusTraversable* GetFocusTraversable();

  void SetFocusTraversableParent(FocusTraversable* parent);
  void SetFocusTraversableParentView(View* parent_view);

  // Clear native focus set to the Widget's NativeWidget.
  void ClearNativeFocus();

  aura::WindowTreeHost* host() { return host_.get();}
  const aura::WindowTreeHost* host() const { return host_.get();}

  // internal
  void OnRootViewSetBoundsChanged(const gfx::Rect& bounds);

  // True if the widget is considered top level widget. Top level widget
  // is a widget of TYPE_WINDOW, TYPE_PANEL, TYPE_WINDOW_FRAMELESS, BUBBLE,
  // POPUP or MENU, and has a focus manager and input method object associated
  // with it. TYPE_CONTROL and TYPE_TOOLTIP is not considered top level.
  bool is_top_level() const { return is_top_level_; }

  // Overridden from views::InputMethodDelegate:
  void DispatchKeyEventPostIME(const ui::KeyEvent& key) override;

  // Sets capture to the specified view. This makes it so that all mouse, touch
  // and gesture events go to |view|. If |view| is NULL, the widget still
  // obtains event capture, but the events will go to the view they'd normally
  // go to.
  void SetCapture(View* view);

  // Releases capture.
  void ReleaseCapture();

  // Returns true if the widget has capture.
  bool HasCapture();

  void set_auto_release_capture(bool auto_release_capture) {
    auto_release_capture_ = auto_release_capture;
  }
 protected:
  // Creates the RootView to be used within this Widget. Subclasses may override
  // to create custom RootViews that do specialized event processing.
  // TODO(beng): Investigate whether or not this is needed.
  virtual internal::RootView* CreateRootView();

  // Creates and initializes a new InputMethod and returns it, otherwise null.
  scoped_ptr<InputMethod> CreateInputMethod();

  // Sets a different InputMethod instance to this widget. The instance
  // must not be initialized, the ownership will be assumed by the widget.
  // It's only for testing purpose.
  void ReplaceInputMethod(InputMethod* input_method);

  void OnNativeThemeUpdated(ui::NativeTheme* observed_theme) override;
  scoped_ptr<aura::WindowTreeHost> host_;
  scoped_ptr<internal::RootView> root_view_;
  scoped_ptr<aura::client::FocusClient> focus_client_;
  scoped_ptr<EventClient> event_client_;
  bool closing_;

  mutable scoped_ptr<InputMethod> input_method_;

  // See |is_top_level()| accessor.
  bool is_top_level_;

  // True if event capture should be released on a mouse up event. Default is
  // true.
  bool auto_release_capture_;

  // A theme provider to use when no other theme provider is specified.
  scoped_ptr<ui::DefaultThemeProvider> default_theme_provider_;

  ScopedObserver<ui::NativeTheme, ui::NativeThemeObserver> observer_manager_;
  DISALLOW_COPY_AND_ASSIGN(Widget);
};
}  // namespace views
