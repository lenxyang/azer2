#pragma once

#include "azer/ui/views/views_export.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/aura/window_delegate.h"

namespace views {

class View;
class RootView;

class VIEWS_EXPORT Window : public aura::Window
             , public aura::WindowDelegate {
 public:
  explicit Window(View* view);
  ~Window() override;

  RootView* GetRootView();

  // from EventHandler
  void OnKeyEvent(ui::KeyEvent* event) override;
  void OnMouseEvent(ui::MouseEvent* event) override;
  void OnScrollEvent(ui::ScrollEvent* event) override;
  void OnTouchEvent(ui::TouchEvent* event) override;
  void OnGestureEvent(ui::GestureEvent* event) override;
 protected:
  View* view_;
  DISALLOW_COPY_AND_ASSIGN(Window);
};

class VIEWS_EXPORT WindowOwner {
 public:
  Window* window();
  const Window* window() const;

  ui::Layer* layer();
 private:
};
}  // namespace views
