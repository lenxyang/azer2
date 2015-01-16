#pragma once

#include "azer/ui/widget/widget_delegate.h"
#include "azer/ui/widget/api.h"

namespace azer {
namespace widget {
namespace testing {
class TestWidgetDelegate : public WidgetDelegate {
 public:
  TestWidgetDelegate();
  ~TestWidgetDelegate() override;

  void set_minimum_size(const gfx::Size& size) {minimum_size_ = size;}
  void set_maximum_size(const gfx::Size& size) {maximum_size_ = size;}
  void set_can_focus(bool can_focus) { can_focus_ = can_focus; }
  
  
  // override from WidgetDelegate
  gfx::Size GetMinimumSize() const override;
  gfx::Size GetMaximumSize() const override;
  bool CanFocus() override;
  bool OnCaptureLost() override;
  void OnPaint(gfx::Canvas* canvas) override;
  bool ShouldDescendIntoChildForEventHandling(Widget* child,
                                              const gfx::Point& location) override;
 private:
  gfx::Size minimum_size_;
  gfx::Size maximum_size_;
  bool can_focus_;
  DISALLOW_COPY_AND_ASSIGN(TestWidgetDelegate);
};

class ColorWidgetDelegate : public TestWidgetDelegate {
 public:
  ColorWidgetDelegate(SkColor color) : color_(color) {}
  ~ColorWidgetDelegate() override {}

  void OnPaint(gfx::Canvas* canvas) override;
 private:
  SkColor color_;
  DISALLOW_COPY_AND_ASSIGN(ColorWidgetDelegate);
};
}  // namespace testing
}  // namespace widget
}  // namespace azer
