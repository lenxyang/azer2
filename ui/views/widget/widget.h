#pragma once

#include "azer/ui/views/views_export.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/aura/window_delegate.h"

namespace views {

class View;

class VIEWS_EXPORT Widget : public aura::Window
             , public aura::WindowDelegate {
 public:
  explicit Widget(View* view);
  ~Widget() override;

  // from EventHandler
  void OnKeyEvent(ui::KeyEvent* event) override;
  void OnMouseEvent(ui::MouseEvent* event) override;
  void OnScrollEvent(ui::ScrollEvent* event) override;
  void OnTouchEvent(ui::TouchEvent* event) override;
  void OnGestureEvent(ui::GestureEvent* event) override;
 protected:
  View* view_;
  DISALLOW_COPY_AND_ASSIGN(Widget);
};

class VIEWS_EXPORT WidgetOwner {
 public:
  Widget* widget();
  const Widget* widget() const;

  ui::Layer* layer();
 private:
};
}  // namespace views
