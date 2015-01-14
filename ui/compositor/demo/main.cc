#include "base/message_loop/message_loop.h"
#include "base/time/time.h"

#include "azer/base/appinit.h"
#include "azer/ui/win/context.h"
#include "azer/ui/win/window_tree_host.h"
#include "azer/ui/compositor.h"
#include "azer/render/render.h"
#include "azer/render/util/render_window.h"

class RenderFrame : public azer::RenderLoop::Delegate {
 public:
  RenderFrame() {}
  ~RenderFrame() {}

  bool Initialize(azer::RenderLoop* renderer) override {
    azer::RenderSystem* rs = azer::RenderSystem::Current();
    overlay_.reset(rs->CreateOverlay());
    compositor_ = new azer::compositor::Compositor;
    compositor_->GetTreeHost()->resize(gfx::Size(800, 600));
  }
  void OnUpdate(const ::base::Time& Time,
                const ::base::TimeDelta& delta) override {
  }
  void OnRender(const ::base::Time& Time,
                const ::base::TimeDelta& delta) override {
    azer::RenderSystem* rs = azer::RenderSystem::Current();
    azer::Renderer* renderer = rs->GetSwapchainRenderer();
    renderer->Use();
    renderer->Clear(azer::Vector4(0.0f, 0.0f, 1.0f, 1.0f));
    renderer->ClearDepthAndStencil();
  }
 private:
  azer::compositor::CompositorPtr compositor_;
  azer::OverlayPtr overlay_;
  DISALLOW_COPY_AND_ASSIGN(RenderFrame);
};

int main(int argc, char* argv[]) {
  ::azer::InitApp(&argc, &argv, "");
  RenderFrame delegate;
  azer::util::RenderWindow window(gfx::Rect(100, 100, 800, 600));
  window.Loop(&delegate);
  return 0;
}
