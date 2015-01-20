#include "azer/ui/widget/widget_tree_host_win.h"

#include "base/message_loop/message_loop.h"
#include "ui/base/cursor/cursor_loader_win.h"
#include "ui/gfx/display.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/screen.h"
#include "ui/platform_window/win/win_window.h"

#include "azer/ui/widget/widget.h"
#include "azer/ui/widget/widget_event_dispatcher.h"

namespace azer {
namespace widget {

// static
gfx::Size WidgetTreeHost::GetNativeScreenSize() {
  return gfx::Size(GetSystemMetrics(SM_CXSCREEN),
                   GetSystemMetrics(SM_CYSCREEN));
}

WidgetTreeHost* WidgetTreeHost::Create(const gfx::Rect& bounds) {
  return new WidgetTreeHostWin(bounds);
}

WidgetTreeHostWin::WidgetTreeHostWin(const gfx::Rect& bounds)
    : WidgetTreeHost(bounds)
    , has_capture_(false)
    , hwnd_(gfx::kNullAcceleratedWidget)
    , window_(new ui::WinWindow(this, bounds)) {
}

WidgetTreeHostWin::~WidgetTreeHostWin() {
  window_.reset();
}

gfx::AcceleratedWidget WidgetTreeHostWin::GetAcceleratedWidget() {
  return hwnd();
}

::ui::EventSource* WidgetTreeHostWin::GetEventSource() {
  return this;
}

::ui::EventProcessor* WidgetTreeHostWin::GetEventProcessor() {
  return dispatcher();
}

void WidgetTreeHostWin::Show() {
  window_->Show();
}

void WidgetTreeHostWin::Hide() {
  window_->Hide();
}
 
gfx::Rect WidgetTreeHostWin::GetBounds() {
  return window_->GetBounds();
}

void WidgetTreeHostWin::SetBounds(const gfx::Rect& bounds) {
  window_->SetBounds(bounds);
}

gfx::Point WidgetTreeHostWin::GetLocationOnNativeScreen() const {
  return window_->GetBounds().origin();
} 

void WidgetTreeHostWin::OnBoundsChanged(const gfx::Rect& new_bounds) {
  gfx::Rect old_bounds = bounds_;
  bounds_ = new_bounds;
  if (bounds_.origin() != old_bounds.origin())
    OnHostMoved(bounds_.origin());
  if (bounds_.size() != old_bounds.size())
    OnHostResized(bounds_.size());
}

void WidgetTreeHostWin::OnDamageRect(const gfx::Rect& damaged_region) {
}

void WidgetTreeHostWin::DispatchEvent(ui::Event* event) {
  ::ui::EventDispatchDetails details = SendEventToProcessor(event);
  if (details.dispatcher_destroyed)
    event->SetHandled();
}

void WidgetTreeHostWin::OnCloseRequest() {
  SetClosed(true);
  base::MessageLoopForUI::current()->Quit();
}

void WidgetTreeHostWin::OnClosed() {
}

void WidgetTreeHostWin::OnWindowStateChanged(ui::PlatformWindowState new_state) {
}

void WidgetTreeHostWin::OnLostCapture() {
  if (has_capture_) {
    has_capture_ = false;
    OnHostLostWindowCapture();
  }
}

void WidgetTreeHostWin::OnAcceleratedWidgetAvailable(gfx::AcceleratedWidget widget) {
  hwnd_ = widget;
  CreateCompositor(widget);
}

void WidgetTreeHostWin::OnActivationChanged(bool active) {
}

void WidgetTreeHostWin::SetCapture() {
  if (!has_capture_) {
    has_capture_ = true;
    window_->SetCapture();
  }
}

void WidgetTreeHostWin::ReleaseCapture() {
  if (has_capture_)
    window_->ReleaseCapture();
}

void WidgetTreeHostWin::SetCursorNative(gfx::NativeCursor native_cursor) {
  // Custom web cursors are handled directly.
  if (native_cursor == ui::kCursorCustom)
    return;

  ui::CursorLoaderWin cursor_loader;
  cursor_loader.SetPlatformCursor(&native_cursor);
  ::SetCursor(native_cursor.platform());
}

}  // namespace widget
}  // namespace azer
