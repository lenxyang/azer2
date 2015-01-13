#pragma once

#include "base/basictypes.h"

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/scoped_observer.h"
#include "ui/events/event_constants.h"
#include "ui/events/event_processor.h"
#include "ui/events/event_targeter.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"

#include "azer/base/export.h"
#include "azer/ui/win/window_observer.h"

namespace azer {
namespace win {

class WindowTreeHost;

class AZER_EXPORT WindowEventDispatcher : public ::ui::EventProcessor,
                                          public WindowObserver {
 public:
  explicit WindowEventDispatcher(WindowTreeHost* host);
// ~WindowEventDispatcher();

  // override ui::EventProcessor,
  ::ui::EventTarget* GetRootTarget() override;
  void OnEventProcessingStarted(::ui::Event* event) override;

  // Overridden from ui::EventDispatcherDelegate.
  // ui::EventProcessor derive from ui::EventDispatcherDelegate
  bool CanDispatchToTarget(::ui::EventTarget* target) override;
  ui::EventDispatchDetails PreDispatchEvent(ui::EventTarget* target,
                                            ui::Event* event) override;
  ui::EventDispatchDetails PostDispatchEvent(ui::EventTarget* target,
                                             const ui::Event& event) override;
 private:
  Window* window();
  const Window* window() const;
  WindowTreeHost* host_;

  void PreDispatchMouseEvent(Window* target, ui::MouseEvent* event);

  Window* event_dispatch_target_;
  Window* old_dispatch_target_;
  DISALLOW_COPY_AND_ASSIGN(WindowEventDispatcher);
};

}  // namespace win
}  // namespace azer
