#include "azer/ui/widget/widget_tree_host.h"

#include "azer/render/render_system.h"
#include "azer/ui/compositor/api.h"
#include "azer/ui/widget/widget.h"
#include "azer/ui/widget/widget_event_dispatcher.h"

namespace azer {
namespace widget {

WidgetTreeHost::WidgetTreeHost(const gfx::Rect& bounds)
    : closed_(false)
    , root_(NULL)
    , bounds_(bounds) {
  dispatcher_.reset(new WidgetEventDispatcher(this));
}

WidgetTreeHost::~WidgetTreeHost() {
  layer_host_.reset();
  compositor_.reset();
}

void WidgetTreeHost::CreateCompositor(gfx::AcceleratedWidget widget) {
  CHECK(RenderSystem::Current() == NULL);
  CHECK(LoadRenderSystem(widget));
  layer_host_.reset(new compositor::LayerTreeHost(bounds_.size()));
  compositor_.reset(new compositor::Compositor);
  layer_host_->SetCompositor(compositor_.get());
  compositor_->SetTreeHost(layer_host_.get());
  root_ = new Widget(this);
}

compositor::Compositor* WidgetTreeHost::compositor() {
  return compositor_.get();
}
}  // namespace widget
}  // namespace azer
