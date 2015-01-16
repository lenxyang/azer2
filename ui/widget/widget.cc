#include "azer/ui/widget/widget.h"

#include "base/logging.h"
#include "ui/events/event_target_iterator.h"
#include "ui/events/event_target.h"
#include "ui/events/event_targeter.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/path.h"
#include "ui/gfx/scoped_canvas.h"
#include "ui/gfx/screen.h"

#include "azer/ui/compositor/api.h"
#include "azer/ui/widget/widget_delegate.h"
#include "azer/ui/widget/widget_context.h"
#include "azer/ui/widget/widget_tree_host.h"

namespace azer {
namespace widget {

Widget::Widget(WidgetType type, Widget* parent) 
    : id_(WidgetContext::GetInstance()->allocate_widget_id())
    , layer_type_(type)
    , host_(parent->host_)
    , parent_(parent) 
    , delegate_(NULL)
    , ignore_events_(false) {
  DCHECK(NULL != parent);
  parent_->AddChild(this);
  InitLayer();
}

Widget::Widget(WidgetTreeHost* host)
    : id_(WidgetContext::GetInstance()->allocate_widget_id())
    , layer_type_(kRoot)
    , host_(host)
    , parent_(NULL) 
    , delegate_(NULL) {
  layer_ = host_->compositor()->GetTreeHost()->root();
}

Widget::~Widget() {
}

void Widget::SetName(const std::string& name) {
  DCHECK(layer_);
  layer_->SetName(name);
}

const std::string& Widget::name() const {
  DCHECK(layer_);
  return layer_->name();
}

void Widget::SetBounds(const gfx::Rect& bounds) {
  bounds_ = bounds;
  layer_->SetBounds(bounds);
}

scoped_ptr<ui::EventTargeter>
Widget::SetEventTargeter(scoped_ptr<ui::EventTargeter> targeter) {
  scoped_ptr<ui::EventTargeter> old_targeter = targeter_.Pass();
  targeter_ = targeter.Pass();
  return old_targeter.Pass();
}

void Widget::AddChild(Widget* widget) {
  DCHECK(widget);
  DCHECK(!Contains(widget));
  children_.push_back(widget);
  widget->parent_ = this;
  widget->host_ = host_;
}

void Widget::RemoveChild(Widget* widget) {
  auto iter = std::find(children_.begin(), children_.end(), widget);
  DCHECK(iter != children_.end());
  children_.erase(iter);
}

bool Widget::Contains(const Widget* widget) const {
  for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
    if (*iter == widget) {
      return true;
    }
  }

  return false;
}

void Widget::SetDelegate(WidgetDelegate* delegate) {
  delegate_ = delegate;
}

void Widget::OnPaintLayer(gfx::Canvas* canvas) {
  if (delegate_) {
    delegate_->OnPaint(canvas);
  }
}

compositor::Layer* Widget::CreateLayerByType() {
  compositor::Layer* layer = NULL;
  switch (type()) {
    case kRoot:
      NOTREACHED();
      break;
    case kNoDraw:
      layer = new compositor::NoDrawLayer(this);
      break;
    case kCanvas:
      layer = new compositor::CanvasLayer(this);
      break;
    case k3DRenderer:
      layer = new compositor::RendererLayer(this);
      break;
    case kBitmap:
      layer = new compositor::BitmapLayer(this);
      break;
    default: layer = NULL;
      break;
  }
  return layer;
}

void Widget::InitLayer() {
  layer_ = CreateLayerByType();
  DCHECK(NULL != layer_);
  if (parent()) {
    DCHECK(parent()->layer_);
    parent()->layer_->Add(layer_);
  }
}

bool Widget::CanAcceptEvent(const ui::Event& event) {
  return true;
}

ui::EventTarget* Widget::GetParentTarget() {
  if (IsRootWidget()) {
    return this;
  }
  return parent_;
}

scoped_ptr<ui::EventTargetIterator> Widget::GetChildIterator() const {
  return scoped_ptr<ui::EventTargetIterator>(
      new ui::EventTargetIteratorImpl<Widget>(children()));
}

ui::EventTargeter* Widget::GetEventTargeter() {
  return targeter_.get();
}

void Widget::ConvertEventToTarget(ui::EventTarget* target,
                                  ui::LocatedEvent* event) {
  event->ConvertLocationToTarget(this,
                                 static_cast<Widget*>(target));
}

// static
void Widget::ConvertRectToTarget(const Widget* source,
                                 const Widget* target,
                                 gfx::Rect* rect) {
  DCHECK(rect);
  gfx::Point origin = rect->origin();
  ConvertPointToTarget(source, target, &origin);
  rect->set_origin(origin);
}

// static
void Widget::ConvertPointToTarget(const Widget* source,
                                  const Widget* target,
                                  gfx::Point* point) {
}
}  // namespace widget
}  // namespace azer
