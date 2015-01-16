
#include "base/message_loop/message_loop.h"
#include "base/time/time.h"
#include "azer/base/appinit.h"
#include "azer/ui/compositor/api.h"
#include "azer/ui/widget/api.h"


using azer::compositor::Compositor;;
using azer::compositor::CanvasLayer;
using azer::compositor::Layer;
using azer::compositor::LayerTreeHost;
using azer::widget::Widget;
using azer::widget::WidgetTreeHost;
using azer::widget::WidgetContext;

class ColorWidgetDelegate : public azer::widget::WidgetDelegate {
 public:
  ColorWidgetDelegate(SkColor color, const gfx::Rect& bounds)
      : color_(color)
      , bounds_(bounds) {
  }

  gfx::Size GetMinimumSize() const override { return bounds_.size();}
  gfx::Size GetMaximumSize() const override { return bounds_.size();}

  bool CanFocus() override { return false; }
  bool OnCaptureLost() override { return false;}

  void OnPaint(gfx::Canvas* canvas) override {
    canvas->DrawColor(color_, SkXfermode::kSrc_Mode);
  }

  bool ShouldDescendIntoChildForEventHandling(
      Widget* child,
      const gfx::Point& location) override {
    return false;
  }
 private:
  SkColor color_;
  gfx::Rect bounds_;
};

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
  ::azer::InitApp(&argc, &argv, "");
  WidgetContext::Init();
  ::base::MessageLoopForUI message_loop;
  
  scoped_ptr<WidgetTreeHost> host(
    WidgetTreeHost::Create(gfx::Rect(100, 100, 800, 600)));
  
  Widget widget1(Widget::kCanvas, host->root());
  widget1.SetName("widget1");
  widget1.SetBounds(gfx::Rect(100, 100, 300, 300));
  ColorWidgetDelegate delegate1(SK_ColorRED, widget1.bounds());
  widget1.SetDelegate(&delegate1);

  Widget widget2(Widget::kCanvas, &widget1);
  widget2.SetName("widget2");
  widget2.SetBounds(gfx::Rect(0, 0, 300, 300));
  ColorWidgetDelegate delegate2(SK_ColorRED, widget2.bounds());
  widget2.SetDelegate(&delegate2);
  
  Widget widget3(Widget::kCanvas, &widget2);
  widget3.SetName("widget3");
  widget3.SetBounds(gfx::Rect(100, 100, 100, 100));
  ColorWidgetDelegate delegate3(SK_ColorRED, widget3.bounds());
  widget3.SetDelegate(&delegate3);
 
  host->Show();
  RenderFrame delegate(host.get());
  azer::widget::RenderLoop renderloop(&delegate);

  renderloop.Run(host.get());
  host.reset();
  WidgetContext::Destroy();
  return 0;
}
