#include "azer/ui/widget/widget_event_dispatcher.h"

#include "base/bind.h"
#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "ui/compositor/dip_util.h"
#include "ui/events/event.h"
#include "azer/ui/widget/widget_tree_host.h"

typedef ui::EventDispatchDetails DispatchDetails;

namespace azer {
namespace widget {
WidgetEventDispatcher::WidgetEventDispatcher(WidgetTreeHost* host)
    : host_(host) {
}

WidgetEventDispatcher::~WidgetEventDispatcher() {
}

ui::EventTarget* WidgetEventDispatcher::GetRootTarget() {
  return host_->root();
}

void WidgetEventDispatcher::OnEventProcessingStarted(ui::Event* event) {
}

bool WidgetEventDispatcher::CanDispatchToTarget(ui::EventTarget* target) {
  return true;
}

ui::EventDispatchDetails WidgetEventDispatcher::PreDispatchEvent(
    ui::EventTarget* target, ui::Event* event) {
  return DispatchDetails();
}

ui::EventDispatchDetails WidgetEventDispatcher::PostDispatchEvent(
    ui::EventTarget* target, const ui::Event& event) {
  return DispatchDetails();
}

Widget* WidgetEventDispatcher::root() {
  return host_->root();
}
}  // namespace widget
}  // namespace azer
