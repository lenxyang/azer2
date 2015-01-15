#include "azer/ui/widget/widget.h"

#include "base/logging.h"
#include "azer/ui/compositor/api.h"
#include "azer/ui/widget/widget_delegate.h"
#include "azer/ui/widget/widget_context.h"
#include "azer/ui/widget/widget_tree_host.h"

namespace azer {
namespace widget {

Widget::Widget(WidgetType type)
    : id_(WidgetContext::GetInstance()->allocate_widget_id())
    , layer_type_(type)
    , host_(NULL)
    , parent_(NULL) 
    , delegate_(NULL) {
  InitLayer();
}

Widget::~Widget() {
}

void Widget::SetName(const std::string& name) {
  DCHECK(layer_.get());
  layer_->SetName(name);
}

const std::string& Widget::name() const {
  DCHECK(layer_.get());
  return layer_->name();
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
      layer = host_->compositor()->GetTreeHost()->root();
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
  layer_.reset(CreateLayerByType());
  DCHECK(NULL != layer_.get());
  if (parent()) {
    DCHECK(parent()->layer_);
    parent()->layer_->Add(layer_.get());
  }
}
}  // namespace widget
}  // namespace azer
