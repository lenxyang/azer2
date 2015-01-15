#include "azer/ui/widget/widget_context.h"

#include "base/logging.h"

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
}

WidgetContext::WidgetContext() 
    : allocated_widget_id_(0) {
}

WidgetContext::~WidgetContext() {
}

int64 WidgetContext::allocate_widget_id() {
  return ++allocated_widget_id_;
}
}  // namespace widget
}  // namespace azer
