#pragma once

#include "base/message_loop/message_loop.h"
#include "ui/events/event_constants.h"
#include "ui/events/event_processor.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "azer/base/export.h"

namespace azer {
namespace widget {

class WidgetTreeHost;

class AZER_EXPORT WidgetEventDispatcher : public ::ui::EventProcessor {
 public:
  WidgetEventDispatcher(WidgetTreeHost* host);
  ~WidgetEventDispatcher() override;
 private:
  // Overridden from ui::EventProcessor:
  ui::EventTarget* GetRootTarget() override;
  void OnEventProcessingStarted(ui::Event* event) override;

  // Overridden from ui::EventDispatcherDelegate.
  bool CanDispatchToTarget(ui::EventTarget* target) override;
  ui::EventDispatchDetails PreDispatchEvent(ui::EventTarget* target,
                                            ui::Event* event) override;
  ui::EventDispatchDetails PostDispatchEvent(ui::EventTarget* target,
                                             const ui::Event& event) override;

  Widget* root();
  DISALLOW_COPY_AND_ASSIGN(WidgetEventDispatcher);
};
}  // namespace widget
}  // namespace azer
