#include "azer/ui/views/view.h"

#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "third_party/skia/include/core/SkRect.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/interpolated_transform.h"
#include "ui/gfx/path.h"
#include "ui/gfx/point3_f.h"
#include "ui/gfx/point_conversions.h"
#include "ui/gfx/scoped_canvas.h"
#include "ui/gfx/screen.h"
#include "ui/gfx/skia_util.h"
#include "ui/gfx/transform.h"

#if defined(OS_WIN)
#include "base/win/scoped_gdi_object.h"
#endif

#include "azer/ui/compositor/layer.h"
#include "azer/ui/compositor/compositor.h"
#include "azer/ui/widget/widget.h"
#include "azer/ui/views/background.h"
#include "azer/ui/views/border.h"
#include "azer/ui/views/painter.h"
#include "azer/ui/views/view_event_observer.h"


namespace azer {
namespace views {

// static
const char View::kViewClassName[] = "View";

View::View() 
    : parent_(NULL)
    , visible_(true)
    , enabled_(true)
    , focusable_(true) {
}

View::View(View* parent) 
    : parent_(NULL)
    , visible_(true)
    , enabled_(true)
    , focusable_(true) {
  parent->AddChildView(this);
  InitWidget();
}

View::~View() {
  event_observers_.Clear();
}

void View::InitWidget() {
  CHECK(!widget_.get());
  DCHECK(parent_);
  widget_.reset(new widget::Widget(parent_->GetWidget()));
  widget_->SetName(name());
  widget_->SetDelegate(this);
}

void View::SetName(const std::string& name)  {
  name_ = name;
  if (widget_.get()) {
    widget_->SetName(name);
  }
}  

void View::AddChildView(View* view) {
  if (view->parent_ == this)
    return;
  AddChildViewAt(view, child_count());
}

void View::AddChildViewAt(View* view, int index) {
  CHECK_NE(view, this) << "You cannot add a view as its own child";
  DCHECK_GE(index, 0);
  DCHECK_LE(index, child_count());
  
  // Let's insert the view.
  view->parent_ = this;
  children_.insert(children_.begin() + index, view);
}

bool View::Contains(const View* view) const {
  for (const View* v = view; v; v = v->parent_) {
    if (v == this)
      return true;
  }
  return false;
}

void View::SetBounds(int x, int y, int width, int height) {
  SetBoundsRect(gfx::Rect(x, y, width, height));
}

void View::SetBoundsRect(const gfx::Rect& bounds) {
  DCHECK(widget_.get());
  widget_->SetBounds(bounds);
  bounds_ = bounds;
}

void View::SetSize(const gfx::Size& size) {
  SetBounds(x(), y(), size.width(), size.height());
}

void View::SetPosition(const gfx::Point& position) {
  SetBounds(position.x(), position.y(), width(), height());
}

void View::SetX(int x) {
  SetBounds(x, y(), width(), height());
}

void View::SetY(int y) {
  SetBounds(x(), y, width(), height());
}

gfx::Rect View::GetContentsBounds() const {
  gfx::Rect contents_bounds(GetLocalBounds());
  if (border_.get()) {
    contents_bounds.Inset(border_->GetInsets());
  }
  return contents_bounds;
}

gfx::Rect View::GetLocalBounds() const {
  return gfx::Rect(size());
}

gfx::Rect View::GetLayerBoundsInPixel() const {
  return layer()->target_bounds();
}

gfx::Insets View::GetInsets() const {
  return border_.get() ? border_->GetInsets() : gfx::Insets();
}

void View::SetEnabled(bool enabled) {
  enabled_ = enabled;
  OnEnabledChanged();
}

void View::set_background(Background* b) {
  background_.reset(b);
}

const char* View::GetClassName() const {
  return kViewClassName;
}

bool View::HasFocus() const {
  return false;
}

void View::SetFocusable(bool focusable) {
  focusable_ = focusable;
}

bool View::IsFocusable() const {
  return focusable_;
}

void View::RequestFocus() {
}

bool View::HitTestPoint(const gfx::Point& point) const {
  return widget_->HitTestPoint(point);
}

bool View::HitTestRect(const gfx::Rect& rect) const {
  return widget_->HitTestRect(rect);
}

const compositor::Layer* View::layer() const {
  DCHECK(widget_.get());
  return widget_->layer();
}

compositor::Layer* View::layer() {
  DCHECK(widget_.get());
  return widget_->layer();
}

void View::SetBorder(scoped_ptr<Border> b) {
  border_ = b.Pass();
}

void View::Show() {
}

void View::Hide() {
}

// Overriden from widget::WidgetDelegate
gfx::Size View::GetMinimumSize() const {
  return GetLocalBounds().size();
}

gfx::Size View::GetMaximumSize() const {
  return GetLocalBounds().size();
}

bool View::CanFocus() {
  return IsFocusable();
}

bool View::OnCaptureLost() {
  return true;
}

// void View::OnPaint(gfx::Canvas* canvas) {
// }

bool View::ShouldDescendIntoChildForEventHandling(widget::Widget* child,
                                                  const gfx::Point& location) {
  return true;
}

void View::SchedulePaint() {
  if (widget_.get()) {
    widget_->SchedulePaint();
  }
}

void View::SchedulePaintInRect(const gfx::Rect& r) {
  if (widget_.get()) {
    widget_->SchedulePaintInRect(r);
  }
}

void View::PaintChildren(gfx::Canvas* canvas, const CullSet& cull_set) {
  TRACE_EVENT1("views", "View::PaintChildren", "class", GetClassName());
  for (int i = 0, count = child_count(); i < count; ++i)
    if (!child_at(i)->layer())
      child_at(i)->Paint(canvas, cull_set);
}

void View::OnPaint(gfx::Canvas* canvas) {
  TRACE_EVENT1("views", "View::OnPaint", "class", GetClassName());
  OnPaintBackground(canvas);
  OnPaintBorder(canvas);
}

void View::OnPaintBackground(gfx::Canvas* canvas) {
  if (background_.get()) {
    TRACE_EVENT2("views", "View::OnPaintBackground",
                 "width", canvas->sk_canvas()->getDevice()->width(),
                 "height", canvas->sk_canvas()->getDevice()->height());
    background_->Paint(canvas, this);
  }
}

void View::OnPaintBorder(gfx::Canvas* canvas) {
  if (border_.get()) {
    TRACE_EVENT2("views", "View::OnPaintBorder",
                 "width", canvas->sk_canvas()->getDevice()->width(),
                 "height", canvas->sk_canvas()->getDevice()->height());
    border_->Paint(*this, canvas);
  }
}

void View::Paint(gfx::Canvas* canvas, const CullSet& cull_set) {
}

bool View::IsMouseHovered() {
  // return HistTestPoint(cursor_pos);
  return false;
}

// events
bool View::OnMousePressed(const ui::MouseEvent& event) {
  FOR_EACH_OBSERVER(ViewEventObserver, event_observers_, OnMousePressed(this, event));
  return false;
}

void View::OnMouseReleased(const ui::MouseEvent& event) {
  FOR_EACH_OBSERVER(ViewEventObserver, event_observers_, OnMouseReleased(this, event));
}

bool View::OnMouseDragged(const ui::MouseEvent& event) {
  FOR_EACH_OBSERVER(ViewEventObserver, event_observers_, OnMouseDragged(this, event));
  return false;
}

void View::OnMouseCaptureLost() {
  FOR_EACH_OBSERVER(ViewEventObserver, event_observers_, OnMouseCaptureLost(this));
}

void View::OnMouseMoved(const ui::MouseEvent& event) {
  FOR_EACH_OBSERVER(ViewEventObserver, event_observers_, OnMouseMoved(this, event));
}

void View::OnMouseEntered(const ui::MouseEvent& event) {
  FOR_EACH_OBSERVER(ViewEventObserver, event_observers_, OnMouseEntered(this, event));
}

void View::OnMouseExited(const ui::MouseEvent& event) {
  FOR_EACH_OBSERVER(ViewEventObserver, event_observers_, OnMouseExited(this, event));
}

bool View::OnKeyPressed(const ui::KeyEvent& event) {
  FOR_EACH_OBSERVER(ViewEventObserver, event_observers_, OnKeyPressed(this, event));
  return false;
}

bool View::OnKeyReleased(const ui::KeyEvent& event) {
  FOR_EACH_OBSERVER(ViewEventObserver, event_observers_, OnKeyReleased(this, event));
  return false;
}

bool View::OnMouseWheel(const ui::MouseWheelEvent& event) {
  FOR_EACH_OBSERVER(ViewEventObserver, event_observers_, OnMouseWheel(this, event));
  return false;
}

// Overridden from ui::EventHandler:
void View::OnKeyEvent(ui::KeyEvent* event) {
  bool consumed = (event->type() == ui::ET_KEY_PRESSED) 
      ? OnKeyPressed(*event)
      : OnKeyReleased(*event);
  if (consumed)
    event->StopPropagation();
}

void View::OnMouseEvent(ui::MouseEvent* event) {
  switch (event->type()) {
    case ui::ET_MOUSE_PRESSED:
      if (OnMousePressed(*event))
        event->SetHandled();
      return;

    case ui::ET_MOUSE_MOVED:
      if ((event->flags() & (ui::EF_LEFT_MOUSE_BUTTON |
                             ui::EF_RIGHT_MOUSE_BUTTON |
                             ui::EF_MIDDLE_MOUSE_BUTTON)) == 0) {
        OnMouseMoved(*event);
        return;
      }
      // FALL-THROUGH
    case ui::ET_MOUSE_DRAGGED:
      return;
    case ui::ET_MOUSE_RELEASED:
      OnMouseReleased(*event);
      return;

    case ui::ET_MOUSEWHEEL:
      if (OnMouseWheel(*static_cast<ui::MouseWheelEvent*>(event)))
        event->SetHandled();
      break;

    case ui::ET_MOUSE_ENTERED:
      OnMouseEntered(*event);
      break;

    case ui::ET_MOUSE_EXITED:
      OnMouseExited(*event);
      break;

    default:
      return;
  }
}

void View::OnScrollEvent(ui::ScrollEvent* event) {
}

void View::OnTouchEvent(ui::TouchEvent* event) {
}

void View::OnGestureEvent(ui::GestureEvent* event) {
}

void View::AddEventObserver(ViewEventObserver* observer) {
  event_observers_.AddObserver(observer);
}

void View::RemoveEventObserver(ViewEventObserver* observer) {
  event_observers_.RemoveObserver(observer);
}

void View::OnVisibleBoundsChanged() {
}

void View::OnVisibilityChanged(bool is_visible) {
}

void View::OnEnabledChanged() {
  SchedulePaint();
}
}  // namespace views
}  // namespace azer
