#include "azer/ui/widget/widget_context.h"

#include "base/logging.h"
#include "ui/events/event_target_iterator.h"
#include "ui/events/event_target.h"
#include "ui/events/event_targeter.h"

namespace azer {
namespace widget {

WidgetContext* WidgetContext::inst_ = NULL;
void WidgetContext::Init() {
  DCHECK(inst_ == NULL);
  inst_ = new WidgetContext();
}

WidgetContext* WidgetContext::GetInstance() {
  DCHECK(inst_ != NULL);
  return inst_;
}

void WidgetContext::Destroy() {
  DCHECK(inst_ != NULL);
  delete inst_;
  inst_ = NULL;
}

WidgetContext::WidgetContext() 
    : allocated_widget_id_(0) {
}

WidgetContext::~WidgetContext() {
}

int64 WidgetContext::allocate_widget_id() {
  return ++allocated_widget_id_;
}

bool WidgetContext::CanAcceptEvent(const ui::Event& event) {
  return true;
}

ui::EventTarget* WidgetContext::GetParentTarget() {
  return NULL;
}

scoped_ptr<ui::EventTargetIterator> WidgetContext::GetChildIterator() const {
  return scoped_ptr<ui::EventTargetIterator>();
}

ui::EventTargeter* WidgetContext::GetEventTargeter() {
  NOTREACHED();
  return NULL;
}

}  // namespace widget
}  // namespace azer
