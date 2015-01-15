#pragma once

#include "base/compiler_specific.h"
#include "base/basictypes.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/point.h"
#include "ui/events/event_source.h"

#include "ui/platform_window/win/win_window.h"
#include "ui/platform_window/platform_window.h"
#include "ui/platform_window/platform_window_delegate.h"

#include "azer/base/export.h"
#include "azer/ui/widget/widget_tree_host.h"


namespace azer {
namespace widget {

class AZER_EXPORT WidgetTreeHostWin 
    : public WidgetTreeHost 
    , public ::ui::EventSource
    , public NON_EXPORTED_BASE(ui::PlatformWindowDelegate) {
 public:
  explicit WidgetTreeHostWin(const gfx::Rect& bounds);
  virtual ~WidgetTreeHostWin();

  gfx::AcceleratedWidget GetAcceleratedWidget() override;
  void Show() override;
  void Hide() override;
  gfx::Rect GetBounds() override;
  void SetBounds(const gfx::Rect& rect) override;
  gfx::Point GetLocationOnNativeScreen() const override;

  ::ui::EventSource* GetEventSource() override;

  // ui::EventSource:
  ::ui::EventProcessor* GetEventProcessor() override;  
 protected:
  gfx::AcceleratedWidget hwnd() const { return hwnd_; }

  // ui::PlatformWindowDelegate:
  void OnBoundsChanged(const gfx::Rect& new_bounds) override;
  void OnDamageRect(const gfx::Rect& damaged_region) override;
  void DispatchEvent(ui::Event* event) override;
  void OnCloseRequest() override;
  void OnClosed() override;
  void OnWindowStateChanged(ui::PlatformWindowState new_state) override;
  void OnLostCapture() override;
  void OnAcceleratedWidgetAvailable(gfx::AcceleratedWidget widget) override;
  void OnActivationChanged(bool active) override;

  gfx::AcceleratedWidget hwnd_;
  scoped_ptr< ::ui::WinWindow> window_;
  DISALLOW_COPY_AND_ASSIGN(WidgetTreeHostWin);
};

}  // namespace widget
}  // namespace azer
