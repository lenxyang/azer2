#pragma once

#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/scoped_observer.h"
#include "ui/base/ui_base_types.h"
#include "ui/events/event_source.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"
#include "ui/native_theme/native_theme_observer.h"

#include "azer/ui/aura/window.h"
#include "azer/ui/aura/window_delegate.h"
#include "azer/ui/views/views_export.h"
#include "azer/ui/views/focus/focus_manager.h"
#include "azer/ui/views/window/client_view.h"
#include "azer/ui/views/window/non_client_view.h"

#if defined(OS_WIN)
// Windows headers define macros for these function names which screw with us.
#if defined(IsMaximized)
#undef IsMaximized
#endif
#if defined(IsMinimized)
#undef IsMinimized
#endif
#if defined(CreateWindow)
#undef CreateWindow
#endif
#endif

namespace base {
class TimeDelta;
}

namespace gfx {
class Canvas;
class Point;
class Rect;
}

namespace ui {
class Accelerator;
class Compositor;
class DefaultThemeProvider;
class InputMethod;
class Layer;
class NativeTheme;
class OSExchangeData;
class ThemeProvider;
}

namespace views {

class View;
class RootView;
class TooltipManager;

class VIEWS_EXPORT Widget {
public:
  enum FrameType {
    FRAME_TYPE_DEFAULT,         // Use whatever the default would be.
    FRAME_TYPE_FORCE_CUSTOM,    // Force the custom frame.
    FRAME_TYPE_FORCE_NATIVE     // Force the native frame.
  };

  // Result from RunMoveLoop().
  enum MoveLoopResult {
    // The move loop completed successfully.
    MOVE_LOOP_SUCCESSFUL,

    // The user canceled the move loop.
    MOVE_LOOP_CANCELED
  };

  // Source that initiated the move loop.
  enum MoveLoopSource {
    MOVE_LOOP_SOURCE_MOUSE,
    MOVE_LOOP_SOURCE_TOUCH,
  };

  // Behavior when escape is pressed during a move loop.
  enum MoveLoopEscapeBehavior {
    // Indicates the window should be hidden.
    MOVE_LOOP_ESCAPE_BEHAVIOR_HIDE,

    // Indicates the window should not be hidden.
    MOVE_LOOP_ESCAPE_BEHAVIOR_DONT_HIDE,
  };

  // Type of visibility change transition that should animate.
  enum VisibilityTransition {
    ANIMATE_SHOW = 0x1,
    ANIMATE_HIDE = 0x2,
    ANIMATE_BOTH = ANIMATE_SHOW | ANIMATE_HIDE,
    ANIMATE_NONE = 0x4,
  };

  explicit Widget();
  ~Widget() override;



    // Returns the bounds of the Widget in screen coordinates.
  gfx::Rect GetWindowBoundsInScreen() const;

  // Returns the bounds of the Widget's client area in screen coordinates.
  gfx::Rect GetClientAreaBoundsInScreen() const;

  // Retrieves the restored bounds for the window.
  gfx::Rect GetRestoredBounds() const;

  // Sizes and/or places the widget to the specified bounds, size or position.
  void SetBounds(const gfx::Rect& bounds);
  void SetSize(const gfx::Size& size);

  // Sizes the window to the specified size and centerizes it.
  void CenterWindow(const gfx::Size& size);

  // Like SetBounds(), but ensures the Widget is fully visible on screen,
  // resizing and/or repositioning as necessary. This is only useful for
  // non-child widgets.
  void SetBoundsConstrained(const gfx::Rect& bounds);

  // Sets whether animations that occur when visibility is changed are enabled.
  // Default is true.
  void SetVisibilityChangedAnimationsEnabled(bool value);

  // Sets the duration of visibility change animations.
  void SetVisibilityAnimationDuration(const base::TimeDelta& duration);

  // Sets the visibility transitions that should animate.
  // Default behavior is to animate both show and hide.
  void SetVisibilityAnimationTransition(VisibilityTransition transition);

  // Starts a nested message loop that moves the window. This can be used to
  // start a window move operation from a mouse or touch event. This returns
  // when the move completes. |drag_offset| is the offset from the top left
  // corner of the window to the point where the cursor is dragging, and is used
  // to offset the bounds of the window from the cursor.
  MoveLoopResult RunMoveLoop(const gfx::Vector2d& drag_offset,
                             MoveLoopSource source,
                             MoveLoopEscapeBehavior escape_behavior);

  // Stops a previously started move loop. This is not immediate.
  void EndMoveLoop();

  // Places the widget in front of the specified widget in z-order.
  void StackAboveWidget(Widget* widget);
  void StackAbove(gfx::NativeView native_view);
  void StackAtTop();

  // Places the widget below the specified NativeView.
  void StackBelow(gfx::NativeView native_view);

  // Sets a shape on the widget. Passing a NULL |shape| reverts the widget to
  // be rectangular. Takes ownership of |shape|.
  void SetShape(gfx::NativeRegion shape);

  // Hides the widget then closes it after a return to the message loop.
  virtual void Close();

  // TODO(beng): Move off public API.
  // Closes the widget immediately. Compare to |Close|. This will destroy the
  // window handle associated with this Widget, so should not be called from
  // any code that expects it to be valid beyond this call.
  void CloseNow();

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

  // Prevents the window from being rendered as deactivated. This state is
  // reset automatically as soon as the window becomes activated again. There is
  // no ability to control the state through this API as this leads to sync
  // problems.
  void DisableInactiveRendering();

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

  // Sets the opacity of the widget. This may allow widgets behind the widget
  // in the Z-order to become visible, depending on the capabilities of the
  // underlying windowing system.
  void SetOpacity(unsigned char opacity);

  // Sets whether or not the window should show its frame as a "transient drag
  // frame" - slightly transparent and without the standard window controls.
  void SetUseDragFrame(bool use_drag_frame);

  // Flashes the frame of the window to draw attention to it. Currently only
  // implemented on Windows for non-Aura.
  void FlashFrame(bool flash);

  // Returns the View at the root of the View hierarchy contained by this
  // Widget.
  View* GetRootView();
  const View* GetRootView() const;

  // A secondary widget is one that is automatically closed (via Close()) when
  // all non-secondary widgets are closed.
  // Default is true.
  // TODO(beng): This is an ugly API, should be handled implicitly via
  //             transience.
  void set_is_secondary_widget(bool is_secondary_widget) {
    is_secondary_widget_ = is_secondary_widget;
  }
  bool is_secondary_widget() const { return is_secondary_widget_; }

  // Returns whether the Widget is visible to the user.
  virtual bool IsVisible() const;

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

  // Starts a drag operation for the specified view. This blocks until the drag
  // operation completes. |view| can be NULL.
  // If the view is non-NULL it can be accessed during the drag by calling
  // dragged_view(). If the view has not been deleted during the drag,
  // OnDragDone() is called on it. |location| is in the widget's coordinate
  // system.
  void RunShellDrag(View* view,
                    const ui::OSExchangeData& data,
                    const gfx::Point& location,
                    int operation,
                    ui::DragDropTypes::DragEventSource source);

  // Returns the view that requested the current drag operation via
  // RunShellDrag(), or NULL if there is no such view or drag operation.
  View* dragged_view() { return dragged_view_; }

  // Adds the specified |rect| in client area coordinates to the rectangle to be
  // redrawn.
  virtual void SchedulePaintInRect(const gfx::Rect& rect);

  // Sets the currently visible cursor. If |cursor| is NULL, the cursor used
  // before the current is restored.
  void SetCursor(gfx::NativeCursor cursor);

  // Returns true if and only if mouse events are enabled.
  bool IsMouseEventsEnabled() const;

  // Sets/Gets a native window property on the underlying native window object.
  // Returns NULL if the property does not exist. Setting the property value to
  // NULL removes the property.
  void SetNativeWindowProperty(const char* name, void* value);
  void* GetNativeWindowProperty(const char* name) const;

  // Tell the window to update its title from the delegate.
  void UpdateWindowTitle();

  // Tell the window to update its icon from the delegate.
  void UpdateWindowIcon();

  // Retrieves the focus traversable for this widget.
  FocusTraversable* GetFocusTraversable();

  // Notifies the view hierarchy contained in this widget that theme resources
  // changed.
  void ThemeChanged();

  // Notifies the view hierarchy contained in this widget that locale resources
  // changed.
  void LocaleChanged();

  void SetFocusTraversableParent(FocusTraversable* parent);
  void SetFocusTraversableParentView(View* parent_view);

  // Clear native focus set to the Widget's NativeWidget.
  void ClearNativeFocus();

  void set_frame_type(FrameType frame_type) { frame_type_ = frame_type; }
  FrameType frame_type() const { return frame_type_; }

  // Creates an appropriate NonClientFrameView for this widget. The
  // WidgetDelegate is given the first opportunity to create one, followed by
  // the NativeWidget implementation. If both return NULL, a default one is
  // created.
  virtual NonClientFrameView* CreateNonClientFrameView();
  
  // Whether we should be using a native frame.
  bool ShouldUseNativeFrame() const;

  // Determines whether the window contents should be rendered transparently
  // (for example, so that they can overhang onto the window title bar).
  bool ShouldWindowContentsBeTransparent() const;

  // Forces the frame into the alternate frame type (custom or native) depending
  // on its current state.
  void DebugToggleFrameType();

  // Tell the window that something caused the frame type to change.
  void FrameTypeChanged();

  NonClientView* non_client_view() {
    return const_cast<NonClientView*>(
        const_cast<const Widget*>(this)->non_client_view());
  }
  const NonClientView* non_client_view() const {
    return non_client_view_;
  }

  ClientView* client_view() {
    return const_cast<ClientView*>(
        const_cast<const Widget*>(this)->client_view());
  }
  const ClientView* client_view() const {
    // non_client_view_ may be NULL, especially during creation.
    return non_client_view_ ? non_client_view_->client_view() : NULL;
  }

  ui::Compositor* GetCompositor() {
    return const_cast<ui::Compositor*>(
        const_cast<const Widget*>(this)->GetCompositor());
  }
  const ui::Compositor* GetCompositor() const;

  // Returns the widget's layer, if any.
  ui::Layer* GetLayer() {
    return const_cast<ui::Layer*>(
        const_cast<const Widget*>(this)->GetLayer());
  }
  const ui::Layer* GetLayer() const;

  // Reorders the widget's child NativeViews which are associated to the view
  // tree (eg via a NativeViewHost) to match the z-order of the views in the
  // view tree. The z-order of views with layers relative to views with
  // associated NativeViews is used to reorder the NativeView layers. This
  // method assumes that the widget's child layers which are owned by a view are
  // already in the correct z-order relative to each other and does no
  // reordering if there are no views with an associated NativeView.
  void ReorderNativeViews();

  // Schedules an update to the root layers. The actual processing occurs when
  // GetRootLayers() is invoked.
  void UpdateRootLayers();

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

  // Returns the font used for tooltips.
  TooltipManager* GetTooltipManager();
  const TooltipManager* GetTooltipManager() const;

  void set_focus_on_creation(bool focus_on_creation) {
    focus_on_creation_ = focus_on_creation;
  }

  // True if the widget is considered top level widget. Top level widget
  // is a widget of TYPE_WINDOW, TYPE_PANEL, TYPE_WINDOW_FRAMELESS, BUBBLE,
  // POPUP or MENU, and has a focus manager and input method object associated
  // with it. TYPE_CONTROL and TYPE_TOOLTIP is not considered top level.
  bool is_top_level() const { return is_top_level_; }

  // True when window movement via mouse interaction with the frame is disabled.
  bool movement_disabled() const { return movement_disabled_; }
  void set_movement_disabled(bool disabled) { movement_disabled_ = disabled; }

  // Returns the work area bounds of the screen the Widget belongs to.
  gfx::Rect GetWorkAreaBoundsInScreen() const;

  // Creates and dispatches synthesized mouse move event using the current
  // mouse location to refresh hovering status in the widget.
  void SynthesizeMouseMoveEvent();

  // Called by our RootView after it has performed a Layout. Used to forward
  // window sizing information to the window server on some platforms.
  void OnRootViewLayout();

  // Whether the widget supports translucency.
  bool IsTranslucentWindowOpacitySupported() const;

  // Called when the delegate's CanResize or CanMaximize changes.
  void OnSizeConstraintsChanged();

  // Notification that our owner is closing.
  // NOTE: this is not invoked for aura as it's currently not needed there.
  // Under aura menus close by way of activation getting reset when the owner
  // closes.
  virtual void OnOwnerClosing();
 protected:
  // The root of the View hierarchy attached to this window.
  // WARNING: see warning in tooltip_manager_ for ordering dependencies with
  // this and tooltip_manager_.
  scoped_ptr<internal::RootView> root_view_;

  // The View that provides the non-client area of the window (title bar,
  // window controls, sizing borders etc). To use an implementation other than
  // the default, this class must be sub-classed and this value set to the
  // desired implementation before calling |InitWindow()|.
  NonClientView* non_client_view_;

  // The focus manager keeping track of focus for this Widget and any of its
  // children.  NULL for non top-level widgets.
  // WARNING: RootView's destructor calls into the FocusManager. As such, this
  // must be destroyed AFTER root_view_. This is enforced in DestroyRootView().
  scoped_ptr<FocusManager> focus_manager_;

  // A theme provider to use when no other theme provider is specified.
  scoped_ptr<ui::DefaultThemeProvider> default_theme_provider_;

  // Valid for the lifetime of RunShellDrag(), indicates the view the drag
  // started from.
  View* dragged_view_;

  // See class documentation for Widget above for a note about ownership.
  InitParams::Ownership ownership_;

  // See set_is_secondary_widget().
  bool is_secondary_widget_;

  // The current frame type in use by this window. Defaults to
  // FRAME_TYPE_DEFAULT.
  FrameType frame_type_;

  // True when the window should be rendered as active, regardless of whether
  // or not it actually is.
  bool disable_inactive_rendering_;

  // Set to true if the widget is in the process of closing.
  bool widget_closed_;

  // The saved "show" state for this window. See note in SetInitialBounds
  // that explains why we save this.
  ui::WindowShowState saved_show_state_;

  // The restored bounds used for the initial show. This is only used if
  // |saved_show_state_| is maximized.
  gfx::Rect initial_restored_bounds_;

  // Focus is automatically set to the view provided by the delegate
  // when the widget is shown. Set this value to false to override
  // initial focus for the widget.
  bool focus_on_creation_;

  mutable scoped_ptr<InputMethod> input_method_;

  // See |is_top_level()| accessor.
  bool is_top_level_;

  // Tracks whether native widget has been initialized.
  bool native_widget_initialized_;

  // Whether native widget has been destroyed.
  bool native_widget_destroyed_;

  // TODO(beng): Remove NativeWidgetGtk's dependence on these:
  // If true, the mouse is currently down.
  bool is_mouse_button_pressed_;

  // True if capture losses should be ignored.
  bool ignore_capture_loss_;

  // TODO(beng): Remove NativeWidgetGtk's dependence on these:
  // The following are used to detect duplicate mouse move events and not
  // deliver them. Displaying a window may result in the system generating
  // duplicate move events even though the mouse hasn't moved.
  bool last_mouse_event_was_move_;
  gfx::Point last_mouse_event_position_;

  // True if event capture should be released on a mouse up event. Default is
  // true.
  bool auto_release_capture_;

  // See description in GetRootLayers().
  std::vector<ui::Layer*> root_layers_;

  // Is |root_layers_| out of date?
  bool root_layers_dirty_;

  // True when window movement via mouse interaction with the frame should be
  // disabled.
  bool movement_disabled_;

  ScopedObserver<ui::NativeTheme, ui::NativeThemeObserver> observer_manager_;
  DISALLOW_COPY_AND_ASSIGN(Widget);
};


}  // namespace views
