#include "azer/ui/widget/widget_tree_host.h"

#include "azer/render/render_system.h"
#include "azer/ui/compositor/api.h"
#include "azer/ui/widget/widget.h"
#include "azer/ui/widget/widget_event_dispatcher.h"

namespace azer {
namespace widget {

WidgetTreeHost::WidgetTreeHost()
    : closed_(false)
    , root_(NULL) {
  dispatcher_.reset(new WidgetEventDispatcher(this));
}

WidgetTreeHost::~WidgetTreeHost() {
  layer_host_.reset();
  compositor_.reset();
}

void WidgetTreeHost::InitCompositor() {
  CHECK(RenderSystem::Current() == NULL);
  CHECK(LoadRenderSystem(GetAcceleratedWidget()));
  layer_host_.reset(new compositor::LayerTreeHost());
  compositor_.reset(new compositor::Compositor);
  layer_host_->SetCompositor(compositor_.get());
  root_ = new Widget(Widget::kRoot);
}

compositor::Compositor* WidgetTreeHost::compositor() {
  return compositor_.get();
}
}  // namespace widget
}  // namespace azer
