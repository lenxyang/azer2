#include "azer/ui/widget/widget_tree_host.h"

#include "ui/gfx/display.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/point.h"
#include "ui/gfx/point3_f.h"
#include "ui/gfx/point_conversions.h"
#include "ui/gfx/screen.h"
#include "ui/gfx/size_conversions.h"

#include "azer/render/render_system.h"
#include "azer/ui/compositor/api.h"
#include "azer/ui/widget/widget.h"
#include "azer/ui/widget/widget_event_dispatcher.h"
#include "azer/ui/widget/widget_targeter.h"
#include "azer/ui/widget/widget_tree_host_observer.h"

namespace azer {
namespace widget {

WidgetTreeHost::WidgetTreeHost(const gfx::Rect& bounds)
    : closed_(false)
    , widget_(NULL)
    , bounds_(bounds)
    , last_cursor_(ui::kCursorNull) {
  dispatcher_.reset(new WidgetEventDispatcher(this));
}

WidgetTreeHost::~WidgetTreeHost() {
  layer_host_.reset();
  compositor_.reset();
  UnloadRenderSystem();
}

void WidgetTreeHost::CreateCompositor(gfx::AcceleratedWidget widget) {
  CHECK(RenderSystem::Current() == NULL);
  CHECK(LoadRenderSystem(widget));
  layer_host_.reset(new compositor::LayerTreeHost(bounds_.size()));
  compositor_.reset(new compositor::Compositor);
  layer_host_->SetCompositor(compositor_.get());
  compositor_->SetTreeHost(layer_host_.get());
  widget_ = new Widget(this);
  widget_->SetName("root");
  widget_->SetEventTargeter(scoped_ptr<ui::EventTargeter>(new WidgetTargeter));
  widget_->SetBounds(gfx::Rect(bounds_.size()));
  layer_host_->SetRoot(widget_->layer());
  dispatcher_.reset(new WidgetEventDispatcher(this));
}

compositor::Compositor* WidgetTreeHost::compositor() {
  return compositor_.get();
}

WidgetEventDispatcher* WidgetTreeHost::dispatcher() {
  return dispatcher_.get();
}

::ui::EventProcessor* WidgetTreeHost::event_processor() {
  return dispatcher();
}

void WidgetTreeHost::OnHostMoved(const gfx::Point& new_location) {
}

void WidgetTreeHost::OnHostResized(const gfx::Size& new_size) {
}

void WidgetTreeHost::OnHostCloseRequested() {
}

void WidgetTreeHost::OnHostActivated() {
}

void WidgetTreeHost::OnHostLostWindowCapture() {
}

void WidgetTreeHost::SetCursor(gfx::NativeCursor cursor) {
  last_cursor_ = cursor;
  SetCursorNative(cursor);
}

void WidgetTreeHost::ConvertPointToNativeScreen(gfx::Point* point) const {
  ConvertPointToHost(point);
  gfx::Point location = GetLocationOnNativeScreen();
  point->Offset(location.x(), location.y());
}

void WidgetTreeHost::ConvertPointFromNativeScreen(gfx::Point* point) const {
  gfx::Point location = GetLocationOnNativeScreen();
  point->Offset(-location.x(), -location.y());
  ConvertPointFromHost(point);
}

void WidgetTreeHost::ConvertPointToHost(gfx::Point* point) const {
  gfx::Point3F point_3f(*point);
  *point = gfx::ToFlooredPoint(point_3f.AsPointF());
}

void WidgetTreeHost::ConvertPointFromHost(gfx::Point* point) const {
  gfx::Point3F point_3f(*point);
  *point = gfx::ToFlooredPoint(point_3f.AsPointF());
}

}  // namespace widget
}  // namespace azer
