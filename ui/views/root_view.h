#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/cursor/cursor.h"
#include "ui/gfx/geometry/rect.h"

#include "azer/ui/aura/window_delegate.h"
#include "azer/ui/aura/window_tree_host_observer.h"
#include "azer/ui/views/view.h"

namespace aura {
class Window;
class WindowTreeHost;

namespace client {
class FocusClient;
}  // namespace client
}  // namespace aura

namespace views {
class VIEWS_EXPORT RootView : public aura::WindowTreeHostObserver,
                              public View {
 public:
  RootView();
  virtual ~RootView();

  struct VIEW_EXPORT InitParams {
    gfx::Rect bounds;
  };
  void Init(const InitParams& param);
  void Close();
  bool IsClosing() const { return closing_;}

  void Show() override;
  void Hide() override;

  aura::WindowTreeHost* host() { return host_.get();}
  const aura::WindowTreeHost* host() const { return host_.get();}
 private:
  scoped_ptr<aura::client::FocusClient> focus_client_;
  scoped_ptr<aura::WindowTreeHost> host_;
  bool closing_;
  DISALLOW_COPY_AND_ASSIGN(RootView);
};
}  // namespace views
