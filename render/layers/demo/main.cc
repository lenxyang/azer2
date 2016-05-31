
#include "base/message_loop/message_loop.h"
#include "base/time/time.h"
#include "azer/base/appinit.h"
#include "azer/ui/compositor/api.h"
#include "azer/ui/widget/api.h"

using azer::compositor::Layer;
using azer::compositor::CanvasLayer;
using azer::compositor::LayerTreeHost;
using azer::widget::WidgetTreeHost;
using azer::widget::WidgetContext;

class ColorLayerDelegate : public azer::compositor::LayerDelegate {
 public:
  ColorLayerDelegate(SkColor color) : color_(color) {};

  virtual void OnPaintLayer(gfx::Canvas* canvas) {
    canvas->DrawColor(color_, SkXfermode::kSrc_Mode);
  }
 private:
  SkColor color_;
  DISALLOW_COPY_AND_ASSIGN(ColorLayerDelegate);
};

class RenderFrame : public azer::widget::RenderLoopDelegate {
 public:
  RenderFrame() {}
  ~RenderFrame() {}

  bool Initialize(azer::widget::RenderLoop* renderer) override {
    azer::RenderSystem* rs = azer::RenderSystem::Current();
    overlay_.reset(rs->CreateOverlay());
    layer_host_.reset(new LayerTreeHost(gfx::Size(800, 600)));
    red_layer_delegate_.reset(new ColorLayerDelegate(SK_ColorRED));
    green_layer_delegate_.reset(new ColorLayerDelegate(SK_ColorGREEN));
    blue_layer_delegate_.reset(new ColorLayerDelegate(SK_ColorBLUE));
    Layer* layer1 = new CanvasLayer(red_layer_delegate_.get());
    layer1->SetName("layer1");
    layer_host_->root()->Add(layer1); 
    layer1->SetBounds(gfx::Rect(100, 100, 300, 300));

    Layer* layer2 = new CanvasLayer(green_layer_delegate_.get());
    layer2->SetName("layer2");
    layer1->Add(layer2); 
    layer2->SetBounds(gfx::Rect(0, 0, 300, 300));
    
    Layer* layer3 = new CanvasLayer(blue_layer_delegate_.get());
    layer3->SetName("layer2");
    layer2->Add(layer3);
    layer3->SetBounds(gfx::Rect(100, 100, 100, 100));

    compositor_.reset(new azer::compositor::Compositor());
    compositor_->SetTreeHost(layer_host_.get());
    return true;
  }

  void OnUpdate(const ::base::Time& Time,
                const ::base::TimeDelta& delta) override {
  }

  void OnRender(const ::base::Time& Time,
                const ::base::TimeDelta& delta) override {
    compositor_->ScheduleDraw();
    compositor_->DoComposite();
    azer::RenderSystem* rs = azer::RenderSystem::Current();
    azer::Renderer* renderer = rs->GetSwapchainRenderer();
    renderer->Use();
    renderer->Clear(azer::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    renderer->ClearDepthAndStencil();
    overlay_->SetTexture(compositor_->GetOutputTexture());
    overlay_->SetBounds(gfx::RectF(-1.0f, -1.0f, 2.0f, 2.0f));
    overlay_->SetTexCoord(gfx::PointF(0.0f, 0.0f), gfx::PointF(1.0f, 1.0f));
    overlay_->Render(renderer);
  }
 private:
  std::unique_ptr<azer::compositor::LayerTreeHost> layer_host_;
  std::unique_ptr<azer::compositor::Compositor> compositor_;
  azer::OverlayPtr overlay_;
  std::unique_ptr<ColorLayerDelegate> red_layer_delegate_;
  std::unique_ptr<ColorLayerDelegate> green_layer_delegate_;
  std::unique_ptr<ColorLayerDelegate> blue_layer_delegate_;
  DISALLOW_COPY_AND_ASSIGN(RenderFrame);
};

int main(int argc, char* argv[]) {
  ::azer::InitApp(&argc, &argv, "");
  WidgetContext::Init();
  ::base::MessageLoopForUI message_loop;
  RenderFrame delegate;
  std::unique_ptr<WidgetTreeHost> host(
    WidgetTreeHost::Create(gfx::Rect(100, 100, 800, 600)));
  azer::widget::RenderLoop renderloop(&delegate);
  host->Show();
  renderloop.Run(host.get());
  host.reset();
  WidgetContext::Destroy();
  return 0;
}
