#pragma once

#pragma once

#include "base/basictypes.h"
#include "base/time/time.h"
#include "azer/ui/widget/widget.h"

namespace gfx {
class Canvas;
}

namespace azer {
class WindowHost;
class RenderSystem;
namespace ui {

class Context;

class AZER_EXPORT WidgetHost : public Widget {
 public:
  WidgetHost(Context* context)
      : Widget(context) {
  }

  virtual ~WidgetHost() {}

  bool Init();
  virtual void Redraw(bool force) override;

  virtual void OnBoundsChanging(const gfx::Rect& newrect) override;
  virtual void OnBoundsChanged(const gfx::Rect& newrect) override;
 protected:
  const static float kMinDrawInterval;
  base::TimeTicks prev_draw_;
  DISALLOW_COPY_AND_ASSIGN(WidgetHost);
};
}  // namespace ui
}  // namespace azer
