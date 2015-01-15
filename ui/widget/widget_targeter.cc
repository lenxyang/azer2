#include "azer/ui/widget/widget_targeter.h"

namespace azer {
namespace widget {
WidgetTargeter::WidgetTargeter() {
}

WidgetTargeter::~WidgetTargeter() {
}

ui::EventTarget* WidgetTargeter::FindTargetForEvent(ui::EventTarget* root,
                                                    ui::Event* event) {
  return root;
}

ui::EventTarget* WidgetTargeter::FindTargetForLocatedEvent(
    ui::EventTarget* root, ui::LocatedEvent* event) {
  return root;
}

bool WidgetTargeter::SubtreeCanAcceptEvent(ui::EventTarget* target,
                                           const ui::LocatedEvent& event) const {
  return false;
}

bool WidgetTargeter::EventLocationInsideBounds(
    ui::EventTarget* target, const ui::LocatedEvent& event) const {
  return false;;
}
}  // namespace widget
}  // namespace azer
