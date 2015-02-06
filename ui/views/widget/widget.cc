#include "azer/ui/views/widget/widget.h"

#include "azer/ui/aura/window_tree_host.h"
#include "azer/ui/aura/window.h"

namespace views {

Widget::Widget(const gfx::Rect& bounds) 
    : cursor_(gfx::kNullCursor) {
  host_->reset(aura::WindowTreeHost::Create(bounds));
  content_window_ = new aura::Window(this);
  content_window_->Init(WINDOW_LAYER_TEXTURED);
  wm::SetShadowType(content_window_, wm::SHADOW_TYPE_NONE);

  content_window_container_ = new aura::Window(NULL);
  content_window_container_->Init(aura::WINDOW_LAYER_NOT_DRAWN);
  content_window_container_->Show();
  content_window_container_->AddChild(content_window_);

  host_->InitHost();
  host_->window()->AddChild(content_window_container_);
  
  host_->AddObserver(this);
}

Widget::~Widget() {
  host_->RemoveObserver(this);
}

bool Widget::IsVisible() {
  return host_->window()->IsVisible();
}

void Widget::UpdateRootLayers() {
}

void Widget::NotifyWillRemoveView(View* view) {
}

RootView* Widget::GetRootView() { 
  return root_view_;
}

const RootView* Widget::GetRootView() const { 
  return root_view_;
}

FocusManager* Widget::GetMocusManager() { 
  return NULL;
}

bool Widget::IsMouseEventsEnabled() { 
  return true;
}

InputMethod* Widget::GetInputMethod() { 
  return NULL;
}

const InputMethod* Widget::GetInputMethod() const { 
  return NULL;
}


// Overridden from aura::WindowDelegate:
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
  return cursor_;
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
}

void Widget::GetHitTestMask(gfx::Path* mask) const {
}


  // Overridden from aura::WindowTreeHostObserver:
void Widget::OnHostCloseRequested(const aura::WindowTreeHost* host) {
}

void Widget::OnHostResized(const aura::WindowTreeHost* host) {
}

void Widget::OnHostMoved(const aura::WindowTreeHost* host,
                         const gfx::Point& new_origin) override {
}

  // Overridden from ui::EventHandler:
void Widget::OnKeyEvent(ui::KeyEvent* event) {
}

void Widget::OnMouseEvent(ui::MouseEvent* event) {
}

void Widget::OnScrollEvent(ui::ScrollEvent* event) {
}

void Widget::OnGestureEvent(ui::GestureEvent* event) {
}

}  // namespace views {
