
#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/i18n/icu_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "third_party/skia/include/core/SkXfermode.h"
#include "azer/ui/aura/client/default_capture_client.h"
#include "azer/ui/aura/client/window_tree_client.h"
#include "azer/ui/aura/env.h"
#include "azer/ui/aura/test/test_focus_client.h"
#include "azer/ui/aura/test/test_screen.h"
#include "azer/ui/aura/window.h"
#include "azer/ui/aura/window_delegate.h"
#include "azer/ui/aura/window_tree_host.h"
#include "ui/base/hit_test.h"
#include "ui/events/event.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/rect.h"
#if defined(USE_X11)
#include "ui/gfx/x/x11_connection.h"
#endif
#if defined(OS_WIN)
#include "ui/gfx/win/dpi.h"
#endif

#include "azer/render/util/render_system_loader.h"

namespace {
// Trivial WindowDelegate implementation that draws a colored background.
class DemoWindowDelegate : public aura::WindowDelegate {
 public:
  explicit DemoWindowDelegate(SkColor color) : color_(color) {}
  // Overridden from WindowDelegate:
  gfx::Size GetMinimumSize() const override { return gfx::Size(); }
  gfx::Size GetMaximumSize() const override { return gfx::Size(); }
  void OnBoundsChanged(const gfx::Rect& old_bounds,
                       const gfx::Rect& new_bounds) override {}
  gfx::NativeCursor GetCursor(const gfx::Point& point) override {
    return gfx::kNullCursor;
  }
  int GetNonClientComponent(const gfx::Point& point) const override {
    return HTCAPTION;
  }
  bool ShouldDescendIntoChildForEventHandling(
      aura::Window* child,
      const gfx::Point& location) override {
    return true;
  }
  bool CanFocus() override { return true; }
  void OnCaptureLost() override {}
  void OnPaint(gfx::Canvas* canvas) override {
    canvas->DrawColor(color_, SkXfermode::kSrc_Mode);
  }
  void OnDeviceScaleFactorChanged(float device_scale_factor) override {}
  void OnWindowDestroying(aura::Window* window) override {}
  void OnWindowDestroyed(aura::Window* window) override {}
  void OnWindowTargetVisibilityChanged(bool visible) override {}
  bool HasHitTestMask() const override { return false; }
  void GetHitTestMask(gfx::Path* mask) const override {}
 private:
  SkColor color_;
  DISALLOW_COPY_AND_ASSIGN(DemoWindowDelegate);
};
class DemoWindowTreeClient : public aura::client::WindowTreeClient {
 public:
  explicit DemoWindowTreeClient(aura::Window* window) : window_(window) {
    aura::client::SetWindowTreeClient(window_, this);
  }
  ~DemoWindowTreeClient() override {
    aura::client::SetWindowTreeClient(window_, NULL);
  }
  // Overridden from aura::client::WindowTreeClient:
  aura::Window* GetDefaultParent(aura::Window* context,
                                 aura::Window* window,
                                 const gfx::Rect& bounds) override {
    if (!capture_client_) {
      capture_client_.reset(
          new aura::client::DefaultCaptureClient(window_->GetRootWindow()));
    }
    return window_;
  }
 private:
  aura::Window* window_;
  scoped_ptr<aura::client::DefaultCaptureClient> capture_client_;
  DISALLOW_COPY_AND_ASSIGN(DemoWindowTreeClient);
};
int DemoMain() {
#if defined(OS_WIN)
  gfx::InitDeviceScaleFactor(1.0f);
#endif
  CHECK(azer::LoadRenderSystem());
  // Create the message-loop here before creating the root window.
  base::MessageLoopForUI message_loop;
  aura::Env::CreateInstance(true);
  scoped_ptr<aura::TestScreen> test_screen(
      aura::TestScreen::Create(gfx::Size()));
  gfx::Screen::SetScreenInstance(gfx::SCREEN_TYPE_NATIVE, test_screen.get());
  scoped_ptr<aura::WindowTreeHost> host(
      test_screen->CreateHostForPrimaryDisplay());
  scoped_ptr<DemoWindowTreeClient> window_tree_client(
      new DemoWindowTreeClient(host->window()));
  aura::test::TestFocusClient focus_client;
  aura::client::SetFocusClient(host->window(), &focus_client);
  // Create a hierarchy of test windows.
  DemoWindowDelegate window_delegate1(SK_ColorBLUE);
  aura::Window window1(&window_delegate1);
  window1.set_id(1);
  window1.Init(aura::WINDOW_LAYER_TEXTURED);
  window1.SetBounds(gfx::Rect(100, 100, 400, 400));
  window1.Show();
  aura::client::ParentWindowWithContext(&window1, host->window(), gfx::Rect());
  DemoWindowDelegate window_delegate2(SK_ColorRED);
  aura::Window window2(&window_delegate2);
  window2.set_id(2);
  window2.Init(aura::WINDOW_LAYER_TEXTURED);
  window2.SetBounds(gfx::Rect(200, 200, 350, 350));
  window2.Show();
  aura::client::ParentWindowWithContext(&window2, host->window(), gfx::Rect());
  DemoWindowDelegate window_delegate3(SK_ColorGREEN);
  aura::Window window3(&window_delegate3);
  window3.set_id(3);
  window3.Init(aura::WINDOW_LAYER_TEXTURED);
  window3.SetBounds(gfx::Rect(10, 10, 50, 50));
  window3.Show();
  window2.AddChild(&window3);
  host->Show();
  while (true) {
    base::MessageLoopForUI::current()->RunUntilIdle();
    host->compositor()->DoComposite();
    host->compositor()->GetSwapChain()->Present();
  }
  return 0;
}
}  // namespace
int main(int argc, char** argv) {
  CommandLine::Init(argc, argv);
  // The exit manager is in charge of calling the dtors of singleton objects.
  base::AtExitManager exit_manager;
  base::i18n::InitializeICU();
  return DemoMain();
}

