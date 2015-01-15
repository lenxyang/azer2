#include "azer/ui/widget/widget_tree_host.h"

#include "azer/ui/compositor/api.h"
#include "azer/ui/widget/widget.h"

namespace azer {
namespace widget {

WidgetTreeHost::WidgetTreeHost() {
  root_ = new Widget(Widget::kRoot);
}

WidgetTreeHost::~WidgetTreeHost() {
  layer_tree_host_.reset();
  compositor_.reset();
}

void WidgetTreeHost::InitCompositor() {
}
}  // namespace widget
}  // namespace azer
