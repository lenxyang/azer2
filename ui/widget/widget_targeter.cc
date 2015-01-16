#include "azer/ui/widget/widget_targeter.h"

#include "ui/events/event_target.h"

#include "azer/ui/widget/widget.h"
#include "azer/ui/widget/widget_delegate.h"
#include "azer/ui/widget/widget_tree_host.h"


namespace azer {
namespace widget {
WidgetTargeter::WidgetTargeter() {
}

WidgetTargeter::~WidgetTargeter() {
}

ui::EventTarget* WidgetTargeter::FindTargetForEvent(ui::EventTarget* root,
                                                    ui::Event* event) {
  Widget* widget = static_cast<Widget*>(root);
  Widget* target = 
      static_cast<Widget*>(EventTargeter::FindTargetForEvent(root, event));
  if (target && !widget->parent() && !widget->Contains(target)) {
    
  }
  return target;
}

ui::EventTarget* WidgetTargeter::FindTargetForLocatedEvent(
    ui::EventTarget* root, ui::LocatedEvent* event) {
  Widget* widget = static_cast<Widget*>(root);
  if (!widget->parent()) {
    Widget* target = FindTargetInRoot(widget, *event);
    if (target) {
      widget->ConvertEventToTarget(target, event);
      return target;
    }
  }
  return ::ui::EventTargeter::FindTargetForLocatedEvent(root, event);
}

bool WidgetTargeter::SubtreeCanAcceptEvent(ui::EventTarget* target,
                                           const ui::LocatedEvent& event) const {
  Widget* widget = static_cast<Widget*>(target);
  if (!widget->IsVisible()) {
    return false;
  }
  if (widget->ignore_events()) {
    return false;
  }
  
  return true;
}

bool WidgetTargeter::EventLocationInsideBounds(
    ui::EventTarget* target, const ui::LocatedEvent& event) const {
  Widget* widget = static_cast<Widget*>(target);
  gfx::Point point = event.location();
  if (widget->parent()) {
    Widget::ConvertPointToTarget(widget->parent(), widget, &point);
  }
  return gfx::Rect(window->bounds().size()).Contains(point);
}

Widget* WidgetTargeter::FindTargetInRoot(Widget* root, 
                                         const ui::LocatedEvent& event) {
  DCHECK_EQ(root, root->root());
  return NULL;
}
}  // namespace widget
}  // namespace azer
