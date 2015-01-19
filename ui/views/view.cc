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

View::~View() {
}

bool View::Contains(const View* view) const {
  for (const View* v = view; v; v = v->parent_) {
    if (v == this)
      return true;
  }
  return false;
}

void View::SetBounds(int x, int y, int width, int height) {
}

void View::SetBoundsRect(const gfx::Rect& bounds) {  
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
}

const char* View::GetClassName() const {
  return kViewClassName;
}

// Overridden from ui::EventHandler:
void View::OnKeyEvent(ui::KeyEvent* event) {
}

void View::OnMouseEvent(ui::MouseEvent* event) {
}

void View::OnScrollEvent(ui::ScrollEvent* event) {
}

void View::OnTouchEvent(ui::TouchEvent* event) {
}

void View::OnGestureEvent(ui::GestureEvent* event) {
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
}  // namespace views
}  // namespace azer
