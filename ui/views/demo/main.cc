
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "azer/render/render.h"
#include "azer/ui/base/environment.h"
#include "azer/ui/widget/api.h"
#include "azer/ui/views/view.h"
#include "azer/ui/views/root_view.h"

class RenderFrame : public azer::widget::RenderLoopDelegate {
 public:
  RenderFrame(WidgetTreeHost* host) : host_(host) {}
  ~RenderFrame() {}

  bool Initialize(azer::widget::RenderLoop* renderer) override {
    azer::RenderSystem* rs = azer::RenderSystem::Current();
    overlay_.reset(rs->CreateOverlay());
    return true;
  }

  void OnUpdate(const ::base::Time& Time,
                const ::base::TimeDelta& delta) override {
  }

  void OnRender(const ::base::Time& Time,
                const ::base::TimeDelta& delta) override {
    Compositor* compositor = host_->compositor();
    compositor->ScheduleDraw();
    compositor->DoComposite();
    azer::RenderSystem* rs = azer::RenderSystem::Current();
    azer::Renderer* renderer = rs->GetSwapchainRenderer();
    renderer->Use();
    renderer->Clear(azer::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    renderer->ClearDepthAndStencil();
    overlay_->SetTexture(compositor->GetOutputTexture());
    overlay_->SetBounds(gfx::RectF(-1.0f, -1.0f, 2.0f, 2.0f));
    overlay_->SetTexCoord(gfx::PointF(0.0f, 0.0f), gfx::PointF(1.0f, 1.0f));
    overlay_->Render(renderer);
  }
 private:
  WidgetTreeHost* host_;
  azer::OverlayPtr overlay_;
  DISALLOW_COPY_AND_ASSIGN(RenderFrame);
};

int main(int argc, char* argv[]) {
  scoped_ptr<azer::UIEnv> uienv;
  uienv.reset(new azer::UIEnv(argc, argv));

  azer::widget::WidgetContext::Init();
  azer::views::RootView root(gfx::Rect(100, 100, 800, 600));
  root->Show();

  RenderFrame delegate(host->GetWidgetTreeHost());
  azer::widget::RenderLoop renderloop(&delegate);

  azer::widget::WidgetContext::Destroy();
  return 0;
}

