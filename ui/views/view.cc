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

#include "azer/ui/widget/widget.h"
#include "azer/ui/views/background.h"
#include "azer/ui/views/border.h"
#include "azer/ui/views/painter.h"


namespace azer {
namespace views {

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
}

gfx::Rect View::GetLocalBounds() const {
}

gfx::Rect View::GetLayerBoundsInPixel() const {
}

gfx::Insets View::GetInsets() const {
}

void View::SetEnabled(bool enabled) {
}

void View::OnPaint(gfx::Canvas* canvas) {
}

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
}  // namespace views
}  // namespace azer
