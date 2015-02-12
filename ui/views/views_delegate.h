// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_VIEWS_DELEGATE_H_
#define UI_VIEWS_VIEWS_DELEGATE_H_

#include <string>

#if defined(OS_WIN)
#include <windows.h>
#endif

#include "base/strings/string16.h"
#include "ui/accessibility/ax_enums.h"
#include "ui/base/ui_base_types.h"
#include "ui/gfx/native_widget_types.h"
#include "azer/ui/views/views_export.h"
#include "azer/ui/views/widget/widget.h"

namespace base {
class TimeDelta;
}

namespace gfx {
class ImageSkia;
class Rect;
}

namespace views {

class View;
class Widget;
class NonClientFrameView;


// ViewsDelegate is an interface implemented by an object using the views
// framework. It is used to obtain various high level application utilities
// and perform some actions such as window placement saving.
//
// The embedding app must set views_delegate to assign its ViewsDelegate
// implementation.
class VIEWS_EXPORT ViewsDelegate {
 public:
#if defined(OS_WIN)
  enum AppbarAutohideEdge {
    EDGE_TOP    = 1 << 0,
    EDGE_LEFT   = 1 << 1,
    EDGE_BOTTOM = 1 << 2,
    EDGE_RIGHT  = 1 << 3,
  };
#endif

  ViewsDelegate();
  virtual ~ViewsDelegate();

  // Saves the position, size and "show" state for the window with the
  // specified name.
  virtual void SaveWindowPlacement(const Widget* widget,
                                   const std::string& window_name,
                                   const gfx::Rect& bounds,
                                   ui::WindowShowState show_state);

  // Retrieves the saved position and size and "show" state for the window with
  // the specified name.
  virtual bool GetSavedWindowPlacement(const Widget* widget,
                                       const std::string& window_name,
                                       gfx::Rect* bounds,
                                       ui::WindowShowState* show_state) const;

  virtual void NotifyAccessibilityEvent(View* view, ui::AXEvent event_type);

  // For accessibility, notify the delegate that a menu item was focused
  // so that alternate feedback (speech / magnified text) can be provided.
  virtual void NotifyMenuItemFocused(const base::string16& menu_name,
                                     const base::string16& menu_item_name,
                                     int item_index,
                                     int item_count,
                                     bool has_submenu);

#if defined(OS_WIN)
  // Retrieves the default window icon to use for windows if none is specified.
  virtual HICON GetDefaultWindowIcon() const;
  // Returns true if the window passed in is in the Windows 8 metro
  // environment.
  virtual bool IsWindowInMetro(gfx::NativeWindow window) const;
#elif defined(OS_LINUX) && !defined(OS_CHROMEOS)
  virtual gfx::ImageSkia* GetDefaultWindowIcon() const;
#endif

  // Creates a default NonClientFrameView to be used for windows that don't
  // specify their own. If this function returns NULL, the
  // views::CustomFrameView type will be used.
  virtual NonClientFrameView* CreateDefaultNonClientFrameView(Widget* widget);

  // AddRef/ReleaseRef are invoked while a menu is visible. They are used to
  // ensure we don't attempt to exit while a menu is showing.
  virtual void AddRef();
  virtual void ReleaseRef();

  // Returns the default obscured text reveal duration.
  virtual base::TimeDelta GetDefaultTextfieldObscuredRevealDuration();

  // Returns true if the operating system's window manager will always provide a
  // title bar with caption buttons (ignoring the setting to
  // |remove_standard_frame| in InitParams). If |maximized|, this applies to
  // maximized windows; otherwise to restored windows.
  virtual bool WindowManagerProvidesTitleBar(bool maximized);

#if defined(OS_WIN)
  // Starts a query for the appbar autohide edges of the specified monitor and
  // returns the current value.  If the query finds the edges have changed from
  // the current value, |callback| is subsequently invoked.  If the edges have
  // not changed, |callback| is never run.
  //
  // The return value is a bitmask of AppbarAutohideEdge.
  virtual int GetAppbarAutohideEdges(HMONITOR monitor,
                                     const base::Closure& callback);
#endif

  // The active ViewsDelegate used by the views system.
  static ViewsDelegate* views_delegate;

 private:
  DISALLOW_COPY_AND_ASSIGN(ViewsDelegate);
};

}  // namespace views

#endif  // UI_VIEWS_VIEWS_DELEGATE_H_
