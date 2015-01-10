#pragma once

#include "base/basictypes.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/point.h"
#include "ui/events/event_source.h"
#include "ui/platform_window/platform_window.h"
#include "ui/platform_window/platform_window_delegate.h"
#include "azer/base/export.h"
#include "azer/ui/win/window_tree_host.h"

namespace azer {
namespace win {

class AZER_EXPORT WindowTreeHostWin 
    : public WindowTreeHost
    , public ui::EventSource
    , public ui::PlatformWindowDelegate {
 public:
  explicit WindowTreeHostWin(const gfx::Rect& bounds);
  virtual ~WindowTreeHostWin();

  void Show() override;
  void Hide() override;
  gfx::Rect GetBounds() override;
  void SetBounds(const gfx::Rect& rect) override;
  gfx::Point GetLocationOnNativeScreen() const override;
  void SetCapture() override;
  void ReleaseCapture() override;
  void SetCursorNative(gfx::NativeCursor cursor) override;
  void MoveCursorToNative(const gfx::Point& location) override;
  void OnCursorVisibilityChangedNative(bool show) override;

  // ui::EventSource:
  ui::EventProcessor* GetEventProcessor() override;
  
 private:
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

  bool has_capture_;
  gfx::Rect bounds_;
  gfx::AcceleratedWidget widget_;
  scoped_ptr<ui::PlatformWindow> window_;

  DISALLOW_COPY_AND_ASSIGN(WindowTreeHostWin);
};
}  // namespace win
}  // namespace azer
