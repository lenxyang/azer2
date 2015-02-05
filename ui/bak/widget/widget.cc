#include "azer/ui/widget/widget.h"

#include "base/logging.h"
#include "ui/events/event_target_iterator.h"
#include "ui/events/event_target.h"
#include "ui/events/event_targeter.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/path.h"
#include "ui/gfx/scoped_canvas.h"
#include "ui/gfx/screen.h"

#include "azer/render/compositor/api.h"
#include "azer/ui/widget/widget_delegate.h"
#include "azer/ui/widget/widget_context.h"
#include "azer/ui/widget/widget_event_dispatcher.h"
#include "azer/ui/widget/widget_observer.h"
#include "azer/ui/widget/widget_property.h"
#include "azer/ui/widget/widget_tracker.h"
#include "azer/ui/widget/widget_tree_host.h"
#include "azer/ui/widget/client/capture_client.h"
#include "azer/ui/widget/client/screen_position_client.h"

namespace azer {
namespace widget {

Widget::Widget(WidgetType type, Widget* parent, int id) 
    : id_(id)
    , layer_type_(type)
    , host_(parent->host_)
    , parent_(NULL) 
    , delegate_(NULL)
    , ignore_events_(false)
    , visible_(true) {
  DCHECK(NULL != parent);
  parent->AddChild(this);
  Init();
}

Widget::Widget(Widget* parent, int id)
    : id_(id)
    , layer_type_(kCanvas)
    , host_(parent->host_)
    , parent_(NULL) 
    , delegate_(NULL)
    , ignore_events_(false)
    , visible_(true) {
  DCHECK(NULL != parent);
  parent->AddChild(this);
  Init();
}

Widget::Widget(WidgetTreeHost* host)
    : id_(1)
    , layer_type_(kRoot)
    , host_(host)
    , parent_(NULL) 
    , delegate_(NULL) {
  layer_ = host_->compositor()->GetTreeHost()->root();
}

Widget::~Widget() {
  FOR_EACH_OBSERVER(WidgetObserver, observers_, OnWidgetDestroying(this));
  host_->dispatcher()->OnPostNotifiedWidgetDestroying(this);
  if (parent_) {
    parent_->RemoveChild(this);
  }
  
  FOR_EACH_OBSERVER(WidgetObserver, observers_, OnWidgetDestroyed(this));
  observers_.Clear();
}

WidgetTreeHost* Widget::GetHost() {
  return host_;
}

const WidgetTreeHost* Widget::GetHost() const {
  return host_;
}

Widget* Widget::GetRootWidget() {
  DCHECK(host_ != NULL);
  return host_->widget();
}

const Widget* Widget::GetRootWidget() const {
  DCHECK(host_ != NULL);
  return host_->widget();
}

void Widget::SetName(const std::string& name) {
  DCHECK(layer_);
  layer_->set_name(name);
}

const std::string& Widget::name() const {
  DCHECK(layer_);
  return layer_->name();
}

compositor::Layer* Widget::layer() {
  return layer_;
}

const compositor::Layer* Widget::layer() const {
  return layer_;
}

void Widget::Hide() {
  SetVisible(false);
}

void Widget::Show() {
  SetVisible(true);
}

void Widget::SetVisible(bool visible) {
  FOR_EACH_OBSERVER(WidgetObserver, observers_,
                    OnWidgetVisibilityChanging(this, visible));
  visible_ = visible;
  FOR_EACH_OBSERVER(WidgetObserver, observers_,
                    OnWidgetVisibilityChanged(this, visible));
}

void Widget::SetBounds(const gfx::Rect& bounds) {
  bounds_ = bounds;
  layer_->SetBounds(bounds);
}

gfx::Rect Widget::GetBoundsInRootWidget() const {
  // TODO(beng): There may be a better way to handle this, and the existing code
  //             is likely wrong anyway in a multi-display world, but this will
  //             do for now.
  if (!GetRootWidget())
    return bounds();
  gfx::Point origin = bounds().origin();
  ConvertPointToTarget(parent_, GetRootWidget(), &origin);
  return gfx::Rect(origin, bounds().size());
}

gfx::Rect Widget::GetBoundsInScreen() const {
  gfx::Rect bounds(GetBoundsInRootWidget());
  const Widget* root = GetRootWidget();
  if (root) {
    client::ScreenPositionClient* screen_position_client =
        client::GetScreenPositionClient(root);
    if (screen_position_client) {
      gfx::Point origin = bounds.origin();
      screen_position_client->ConvertPointToScreen(root, &origin);
      bounds.set_origin(origin);
    }
  }
  return bounds;
}

scoped_ptr<ui::EventTargeter>
Widget::SetEventTargeter(scoped_ptr<ui::EventTargeter> targeter) {
  scoped_ptr<ui::EventTargeter> old_targeter = targeter_.Pass();
  targeter_ = targeter.Pass();
  return old_targeter.Pass();
}

void Widget::AddChild(Widget* widget) {
  DCHECK(widget);
  DCHECK(!widget->parent_);
  children_.push_back(widget);
  widget->parent_ = this;
  widget->host_ = host_;
  GetHost()->dispatcher()->OnWidgetAddedToRootWidget(widget);
  widget->NotifyAddedToRootWidget();
}

void Widget::RemoveChild(Widget* child) {
  child->NotifyRemovingFromRootWidget();
  auto iter = std::find(children_.begin(), children_.end(), child);
  DCHECK(iter != children_.end());
  children_.erase(iter);
}

bool Widget::Contains(const Widget* other) const {
  for (const Widget* parent = other; parent; parent = parent->parent_) {
    if (parent == this)
      return true;
  }
  return false;
}

void Widget::SetDelegate(WidgetDelegate* delegate) {
  delegate_ = delegate;
  set_target_handler(delegate_);
}

void Widget::OnPaintLayer(gfx::Canvas* canvas) {
  if (delegate_) {
    delegate_->OnPaint(canvas);
  }
}

compositor::Layer* Widget::CreateLayerByType() {
  compositor::Layer* layer = NULL;
  switch (layer_type_) {
    case kRoot:
      NOTREACHED();
      break;
    case kNoDraw:
      layer = new compositor::NoDrawLayer();
      break;
    case kCanvas:
      layer = new compositor::CanvasLayer();
      break;
    case k3DRenderer:
      layer = new compositor::RendererLayer();
      break;
    case kBitmap:
      layer = new compositor::BitmapLayer();
      break;
    default: layer = NULL;
      break;
  }
  layer->set_delegate(this);
  return layer;
}

void Widget::Init() {
  if (id_ == -1) {
    id_ = WidgetContext::GetInstance()->allocate_widget_id();
  }

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
    return WidgetContext::GetInstance();
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
  if (!source)
    return;
  
  DCHECK(target != NULL);
  DCHECK(target->layer() != NULL);
  
  compositor::Layer::ConvertPointToLayer(source->layer(), target->layer(), point);
}

void Widget::SchedulePaint() {
  DCHECK(layer());
  layer()->SetNeedRedraw(bounds());
}

void Widget::SchedulePaintInRect(const gfx::Rect& r) {
  DCHECK(layer());
  layer()->SetNeedRedraw(r);
}

// capture 
void Widget::SetCapture() {
  if (!IsVisible()) {
    return;
  }

  client::CaptureClient* capture_client = client::GetCaptureClient(GetRootWidget());
  if (!capture_client) {
    return;
  }
  
  client::GetCaptureClient(GetRootWidget())->SetCapture(this);
}

void Widget::ReleaseCapture() {
  client::CaptureClient* capture_client = client::GetCaptureClient(GetRootWidget());
  if (!capture_client) {
    return;
  }
  client::GetCaptureClient(GetRootWidget())->ReleaseCapture(this);
}

bool Widget::HasCapture() {
  client::CaptureClient* capture_client = client::GetCaptureClient(GetRootWidget());
  return capture_client && capture_client->GetCaptureWidget() == this;
}

int64 Widget::SetPropertyInternal(const void* key,
                                  const char* name,
                                  PropertyDeallocator deallocator,
                                  int64 value,
                                  int64 default_value) {
  int64 old = GetPropertyInternal(key, default_value);
  if (value == default_value) {
    prop_map_.erase(key);
  } else {
    Value prop_value;
    prop_value.name = name;
    prop_value.value = value;
    prop_value.deallocator = deallocator;
    prop_map_[key] = prop_value;
  }
  FOR_EACH_OBSERVER(WidgetObserver, observers_,
                    OnWidgetPropertyChanged(this, key, old));
  return old;
}

int64 Widget::GetPropertyInternal(const void* key,
                                  int64 default_value) const {
  std::map<const void*, Value>::const_iterator iter = prop_map_.find(key);
  if (iter == prop_map_.end())
    return default_value;
  return iter->second.value;
}

void Widget::AddObserver(WidgetObserver* observer) {
  observer->OnWidgetObserving(this);
  observers_.AddObserver(observer);
}

void Widget::RemoveObserver(WidgetObserver* observer) {
  observer->OnWidgetUnobserving(this);
  observers_.RemoveObserver(observer);
}

bool Widget::HasObserver(WidgetObserver* observer) {
  return observers_.HasObserver(observer);
}

bool Widget::HitTestPoint(const gfx::Point& point) const {
  return HitTestRect(gfx::Rect(point, gfx::Size(1, 1)));
}

bool Widget::HitTestRect(const gfx::Rect& rect) const {
  return true;
}

void Widget::StackChildAtTop(Widget* child) {
  if (children_.size() <= 1 || child == children_.back())
    return;  // In the front already.
  StackChildAbove(child, children_.back());
}

void Widget::StackChildAbove(Widget* child, Widget* target) {
  StackChildRelativeTo(child, target, STACK_ABOVE);
}

void Widget::StackChildAtBottom(Widget* child) {
  if (children_.size() <= 1 || child == children_.front())
    return;  // At the bottom already.
  StackChildBelow(child, children_.front());
}

void Widget::StackChildBelow(Widget* child, Widget* target) {
  StackChildRelativeTo(child, target, STACK_BELOW);
}

void Widget::StackChildRelativeTo(Widget* child, Widget* target,
                                  StackDirection direction) {
  DCHECK_NE(child, target);
  DCHECK(child);
  DCHECK(target);
  DCHECK_EQ(this, child->parent());
  DCHECK_EQ(this, target->parent());

  const size_t child_i =
      std::find(children_.begin(), children_.end(), child) - children_.begin();
  const size_t target_i =
      std::find(children_.begin(), children_.end(), target) - children_.begin();

  // Don't move the child if it is already in the right place.
  if ((direction == STACK_ABOVE && child_i == target_i + 1) ||
      (direction == STACK_BELOW && child_i + 1 == target_i))
    return;

  const size_t dest_i =
      direction == STACK_ABOVE ?
      (child_i < target_i ? target_i : target_i + 1) :
      (child_i < target_i ? target_i - 1 : target_i);
  children_.erase(children_.begin() + child_i);
  children_.insert(children_.begin() + dest_i, child);
  
  StackChildLayerRelativeTo(child, target, direction);

  child->OnStackingChanged();
}

void Widget::StackChildLayerRelativeTo(Widget* child, Widget* target, 
                                       StackDirection direction) {
  compositor::Layer* parent = layer();
  DCHECK(parent == child->layer()->parent());
  DCHECK(parent == target->layer()->parent());
  if (direction == STACK_ABOVE) {
    parent->StackAbove(child->layer(), target->layer());
  } else {
    parent->StackBelow(child->layer(), target->layer());
  }
}

void Widget::OnStackingChanged() {
  FOR_EACH_OBSERVER(WidgetObserver, observers_, OnWidgetStackingChanged(this));
}

void Widget::NotifyAddedToRootWidget() {
  FOR_EACH_OBSERVER(WidgetObserver, observers_, OnWidgetAddedToRootWidget(this));
  for (auto it = children_.begin(); it != children_.end(); ++it) {
    (*it)->NotifyAddedToRootWidget();
  }
}

void Widget::NotifyRemovingFromRootWidget() {
  FOR_EACH_OBSERVER(WidgetObserver, observers_,
                    OnWidgetRemovingFromRootWidget(this));
  for (auto it = children_.begin(); it != children_.end(); ++it) {
    (*it)->NotifyRemovingFromRootWidget();
  }
}

void Widget::NotifyWidgetVisiblityChanged(Widget* target, bool visible) {
  if (!NotifyWidgetVisibilityChangedDown(target, visible)) {
    return; // |this| has been deleted.
  }
  NotifyWidgetVisibilityChangedUp(target, visible);
}

bool Widget::NotifyWidgetVisibilityChangedAtReceiver(Widget* target, bool visible) {
  // |this| may be deleted during a call to OnWidgetVisibilityChanged() on one
  // of the observers. We create an local observer for that. In that case we
  // exit without further access to any members.
  WidgetTracker tracker;
  tracker.Add(this);
  FOR_EACH_OBSERVER(WidgetObserver, observers_, 
                    OnWidgetVisibilityChanged(target, visible));
  return tracker.Contains(this);
}

bool Widget::NotifyWidgetVisibilityChangedDown(Widget* target, bool visible) {
  if (!NotifyWidgetVisibilityChangedAtReceiver(target, visible))
    return false; // |this| was deleted.
  std::set<const Widget*> child_already_processed;
  bool child_destroyed = false;
  do {
    child_destroyed = false;
    for (auto it = children_.begin(); it != children_.end(); ++it) {
      if (!child_already_processed.insert(*it).second)
        continue;
      if (!(*it)->NotifyWidgetVisibilityChangedDown(target, visible)) {
        // |*it| was deleted, |it| is invalid and |children_| has changed.
        // We exit the current for-loop and enter a new one.
        child_destroyed = true;
        break;
      }
    }
  } while (child_destroyed);
  return true;
}

void Widget::NotifyWidgetVisibilityChangedUp(Widget* target, bool visible) {
  // Start with the parent as we already notified |this|
  // in NotifyWidgetVisibilityChangedDown.
  for (Widget* widget = parent(); widget; widget = widget->parent()) {
    bool ret = widget->NotifyWidgetVisibilityChangedAtReceiver(target, visible);
    DCHECK(ret);
  }
}

bool Widget::ContainsPointInRoot(const gfx::Point& point_in_root) const {
  const Widget* root_widget = GetRootWidget();
  if (!root_widget)
    return false;
  gfx::Point local_point(point_in_root);
  ConvertPointToTarget(root_widget, this, &local_point);
  return gfx::Rect(bounds().size()).Contains(local_point);
}

bool Widget::ContainsPoint(const gfx::Point& local_point) const {
  return gfx::Rect(bounds().size()).Contains(local_point);
}
}  // namespace widget
}  // namespace azer
