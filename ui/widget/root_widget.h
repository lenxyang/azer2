#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/cursor/cursor.h"
#include "ui/gfx/geometry/rect.h"

#include "azer/ui/aura/window_delegate.h"
#include "azer/ui/aura/window_tree_host_observer.h"
#include "azer/ui/widget/widget.h"

namespace aura {
class Window;
class WindowTreeHost;
}  // namespace aura

namespace widget {
class WIDGET_EXPORT RootWidget : public aura::WindowTreeHostObserver,
                                 public Widget {
 public:
  RootWidget();
  virtual ~RootWidget();
  struct WIDGET_EXPORT InitParams {
    gfx::Rect bounds;
  };
  void Init(const InitParams& param);
  void Close();
  bool closing() const { return closing_;}

  aura::WindowTreeHost* host() { return host_.get();}
  const aura::WindowTreeHost* host() const { return host_.get();}
 private:
  scoped_ptr<WindowTreeHost> host_;
  bool closing_;
  DISALLOW_COPY_AND_ASSIGN(RootWidget);
};
}  // namespace widget
