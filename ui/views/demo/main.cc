
#include "base/memory/scoped_ptr.h"
#include "base/strings/utf_string_conversions.h"
#include "base/strings/stringprintf.h"

#include "azer/render/render.h"
#include "azer/ui/base/environment.h"
#include "azer/ui/widget/api.h"
#include "azer/ui/compositor/api.h"
#include "azer/ui/views/controls.h"
#include "azer/ui/views/view_event_observer.h"

using azer::compositor::Compositor;
using azer::widget::WidgetTreeHost;
using azer::views::RootView;
using azer::views::Label;
using azer::widget::RenderLoop;
using azer::views::View;

class LabelEventObserver : public azer::views::ViewEventObserver {
 public:
  LabelEventObserver(Label* label1, Label* label2)
      : label1_(label1)
      , label2_(label2) {
  }

  virtual void OnMousePressed(View* view, const ui::MouseEvent& e) {
    std::string msg = ::base::StringPrintf("OnMousePressed on view[%s]",
                                            view->name());
    label1_->SetText(::base::UTF8ToWide(msg));
  }
  virtual void OnMouseReleased(View* view, const ui::MouseEvent& e) {
    std::string msg = ::base::StringPrintf("OnMouseReleased on view[%s]",
                                           view->name());
    label1_->SetText(::base::UTF8ToWide(msg));
  }

  virtual void OnMouseMoved(View* view, const ui::MouseEvent& e) {
    std::string msg = ::base::StringPrintf("OnMouseMoved on view[%s]",
                                            view->name());
    label2_->SetText(::base::UTF8ToWide(msg));
  }
  virtual void OnMouseEntered(View* view, const ui::MouseEvent& e) {
    std::string msg = ::base::StringPrintf("OnMouseEntered on view[%s]",
                                           view->name());
    label1_->SetText(::base::UTF8ToWide(msg));
  }
  virtual void OnMouseExited(View* view, const ui::MouseEvent& e) {
    std::string msg = ::base::StringPrintf("OnMouseExited on view[%s]",
                                           view->name());
    label1_->SetText(::base::UTF8ToWide(msg));
  }

  virtual void OnMouseWheel(View* view, const ui::MouseWheelEvent& e) {
    std::string msg = ::base::StringPrintf("OnMouseWheel on view[%s]",
                                           view->name());
    label1_->SetText(::base::UTF8ToWide(msg));
  }
  virtual void OnKeyPressed(View* view, const ui::KeyEvent& e) {
    std::string msg = ::base::StringPrintf("OnKeyPressed on view[%s]",
                                           view->name());
    label1_->SetText(::base::UTF8ToWide(msg));
  }
  virtual void OnKeyReleased(View* view, const ui::KeyEvent& e) {
    std::string msg = ::base::StringPrintf("OnKeyReleased on view[%s]",
                                           view->name());
    label1_->SetText(::base::UTF8ToWide(msg));
  }
 private:
  Label* label1_;
  Label* label2_;
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
  scoped_ptr<azer::UIEnv> uienv;
  uienv.reset(new azer::UIEnv(argc, argv));
  
  azer::widget::WidgetContext::Init();
  scoped_ptr<RootView> root(new RootView(gfx::Rect(100, 100, 800, 600)));
  View* panel = new View;
  root->AddChildView(panel);
  panel->SetBounds(40, 40, 100, 100);
  Label* label = new Label(::base::UTF8ToWide("This is a Label"));
  root->AddChildView(label);
  label->SetBounds(10, 10, 200, 20);
  root->Show();

  scoped_ptr<RenderFrame> delegate(new RenderFrame(root->GetWidgetTreeHost()));
  RenderLoop renderloop(delegate.get());
  renderloop.Run(root->GetWidgetTreeHost());

  root.reset();
  delegate.reset();
  azer::widget::WidgetContext::Destroy();
  return 0;
}

