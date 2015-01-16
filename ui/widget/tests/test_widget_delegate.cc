#include "azer/ui/widget/tests/test_widget_delegate.h"

namespace azer {
namespace widget {
namespace testing {

TestWidgetDelegate::TestWidgetDelegate()
    : can_focus_(true) {
}

TestWidgetDelegate::~TestWidgetDelegate() {
}

gfx::Size TestWidgetDelegate::GetMinimumSize() const {
  return minimum_size_;
}

gfx::Size TestWidgetDelegate::GetMaximumSize() const {
  return maximum_size_;
}

bool TestWidgetDelegate::CanFocus() {
  return can_focus_;
}

bool TestWidgetDelegate::OnCaptureLost() {
  return true;
}

void TestWidgetDelegate::OnPaint(gfx::Canvas* canvas) {
}

bool TestWidgetDelegate::ShouldDescendIntoChildForEventHandling( 
    Widget* child, const gfx::Point& location) {
  return true;
}

void ColorWidgetDelegate::OnPaint(gfx::Canvas* canvas) {
  canvas->DrawColor(color_, SkXfermode::kSrc_Mode);
}
   
}  // namespace testing
}  // namespace widget
}  // namespace azer
