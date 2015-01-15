#include "azer/ui/widget/widget_tree_host_win.h"

#include "base/message_loop/message_loop.h"
#include "ui/gfx/display.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/screen.h"
#include "ui/platform_window/win/win_window.h"

#include "azer/ui/widget/widget.h"

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
    : hwnd_(gfx::kNullAcceleratedWidget)
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
  return NULL;
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
}

void WidgetTreeHostWin::OnAcceleratedWidgetAvailable(gfx::AcceleratedWidget widget) {
  hwnd_ = widget;
  InitCompositor();
}

void WidgetTreeHostWin::OnActivationChanged(bool active) {
}

}  // namespace widget
}  // namespace azer
