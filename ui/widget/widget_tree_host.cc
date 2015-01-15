#include "azer/ui/widget/widget_tree_host.h"

#include "azer/ui/compositor/api.h"
#include "azer/ui/widget/widget.h"

namespace azer {
namespace widget {

WidgetTreeHost::WidgetTreeHost()
    : closed_(false)
    , root_(NULL) {
  
}

WidgetTreeHost::~WidgetTreeHost() {
  layer_host_.reset();
  compositor_.reset();
}

void WidgetTreeHost::InitCompositor() {
  layer_host_.reset(new compositor::LayerTreeHost(GetBounds().size()));
  compositor_.reset(new compositor::Compositor);
  layer_host_->SetCompositor(compositor_.get());
  root_ = new Widget(Widget::kRoot);
}

compositor::Compositor* WidgetTreeHost::compositor() {
  return compositor_.get();
}
}  // namespace widget
}  // namespace azer
