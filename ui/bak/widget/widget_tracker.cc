#include "azer/ui/widget/widget_tracker.h"

#include "azer/ui/widget/widget.h"

namespace azer {
namespace widget {

WidgetTracker::WidgetTracker() {
}

WidgetTracker::~WidgetTracker() {
  for (Widgets::iterator i = widgets_.begin(); i != widgets_.end(); ++i)
    (*i)->RemoveObserver(this);
}

void WidgetTracker::Add(Widget* widget) {
  if (widgets_.count(widget))
    return;

  widget->AddObserver(this);
  widgets_.insert(widget);
}

void WidgetTracker::Remove(Widget* widget) {
  if (widgets_.count(widget)) {
    widgets_.erase(widget);
    widget->RemoveObserver(this);
  }
}

bool WidgetTracker::Contains(Widget* widget) {
  return widgets_.count(widget) > 0;
}

void WidgetTracker::OnWidgetDestroying(Widget* widget) {
  DCHECK_GT(widgets_.count(widget), 0u);
  Remove(widget);
}

}  // namespace widget
}  // namespace azer
