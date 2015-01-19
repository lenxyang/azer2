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

  widget::WidgetTreeHost* GetWidgetTreeHost() { return host_.get();}
 private:
  scoped_ptr<widget::WidgetTreeHost> host_;
  DISALLOW_COPY_AND_ASSIGN(RootView);
};
}  // namespace views
}  // namespace azer
