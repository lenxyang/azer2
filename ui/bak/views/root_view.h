#pragma once

#include "azer/ui/views/view.h"

namespace azer {

namespace widget {
class WidgetTreeHost;
}

namespace views {
class AZER_EXPORT RootView : public View {
 public:
  explicit RootView(const gfx::Rect& bounds);
  ~RootView() override;

  void Show() override;
  void Hide() override;

  widget::WidgetTreeHost* GetWidgetTreeHost();

  bool OnMousePressed(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnMouseMoved(const ui::MouseEvent& event) override;
  void OnMouseEntered(const ui::MouseEvent& event) override;
  void OnMouseExited(const ui::MouseEvent& event) override;
 protected:
  void OnPaint(gfx::Canvas* canvas) override;
  scoped_ptr<widget::WidgetTreeHost> host_;
 private:
  DISALLOW_COPY_AND_ASSIGN(RootView);
};
}  // namespace views
}  // namespace azer
