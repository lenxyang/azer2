#include "azer/ui/widget/widget.h"

#include <atomic>
#include "base/logging.h"
#include "base/lazy_instance.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/aura/window_property.h"

DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(WIDGET_EXPORT, widget::Widget*)
DECLARE_WINDOW_PROPERTY_TYPE(widget::Widget*)

namespace widget {


namespace {
class AuraIdAllocator {
 public:
  AuraIdAllocator() : allocated_id_(0) {}

  int64 allocate_id() { return allocated_id_++;}
 protected:
  std::atomic<int64> allocated_id_;
  DISALLOW_COPY_AND_ASSIGN(AuraIdAllocator);
};

::base::LazyInstance<AuraIdAllocator> id_allocator_ = LAZY_INSTANCE_INITIALIZER;
}  // namespace

Widget::Widget() {
  window_ = new aura::Window(this);
  window_->Init(aura::WINDOW_LAYER_TEXTURED);
  window_->set_id(id_allocator_.Pointer()->allocate_id());
  window_->SetName(kViewClassName);
}

Widget::~Widget() {
}

int Widget::id() const {
  return window_->id();
}

void Widget::set_id(int id) {
  window_->set_id(id);
}

const std::string& name() const {
  return window_->name();
}

void Widget::SetName(const std::string& name) {
  window_->SetName(name);
}

void Widget::Show() {
  window()->Show();
}

void Widget::Hide() {
  window()->Hide();
}

void Widget::AddChild(Widget* widget) {
  DCHECK(widget->parent_ == NULL);
  widget->parent_ = this;
  children_.push_back(widget);

  window()->AddChild(widget->window());
}

void Widget::RemoveChild(Widget* child) {
  DCHECK(widget->parent_ == this);
  std::vector<Layer*>::iterator i =
    std::find(children_.begin(), children_.end(), child);
  DCHECK(i != children_.end());
  children_.erase(i);

  window()->RemoveChild(child->window());
}

bool Widget::Contains(Widget* widget) {
  return window()->Contains(widget->window());
}

gfx::Size Widget::GetMinimumSize() const {
  return gfx::Size(1, 1);
}

gfx::Size Widget::GetMaximumSize() const {
  return gfx::Size(800, 600);
}

void Widget::OnBoundsChanged(const gfx::Rect& old_bounds,
                             const gfx::Rect& new_bounds)  {
}

gfx::NativeCursor Widget::GetCursor(const gfx::Point& point) {
  return gfx::kNullCursor;
}

int Widget::GetNonClientComponent(const gfx::Point& point) const {
  return 0;
}

bool Widget::ShouldDescendIntoChildForEventHandling(aura::Window* child,
                                                    const gfx::Point& location) {
  return true;
}

bool Widget::CanFocus() {
  return false;
}

void Widget::OnCaptureLost() {
}

void Widget::OnPaint(gfx::Canvas* canvas) {
}

void Widget::OnDeviceScaleFactorChanged(float device_scale_factor) {
}

void Widget::OnWindowDestroying(aura::Window* window) {
}

void Widget::OnWindowDestroyed(aura::Window* window) {
}

void Widget::OnWindowTargetVisibilityChanged(bool visible) {
}

bool Widget::HasHitTestMask() const {
  return false;
}

void Widget::GetHitTestMask(gfx::Path* mask) const {
}

void Widget::OnKeyEvent(ui::KeyEvent* event) {
}

void Widget::OnMouseEvent(ui::MouseEvent* event) {
}

void Widget::OnScrollEvent(ui::ScrollEvent* event) {
}

void Widget::OnTouchEvent(ui::TouchEvent* event) {
}

void Widget::OnGestureEvent(ui::GestureEvent* event) {
}

}  // namespace widget
