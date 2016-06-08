#pragma once

#include "ui/views/widget/widget_observer.h"

#include "azer/render/frame_args.h"
#include "azer/render/render_system.h"
#include "azer/render/renderer.h"
#include "azer/ui/adapter/swapchain_context.h"
#include "azer/ui/window.h"

namespace azer {

class RenderDelegate;
class RenderLoop;
class RenderView;
class WidgetRendererContext;

class RenderWindow : public Window,
                     public views::WidgetObserver {
 public:
  static const char kViewClassName[];
  RenderWindow(RenderView* view, RenderDelegate* delegate);
  ~RenderWindow() override;

  SwapChainPtr& GetSwapChain();
  RendererPtr& GetRenderer();
  bool Init(RenderLoop* render_loop);
  // called by RenderLoop, init render for first time
  void FirstRender();
  void Render(const FrameArgs& frame_data);
  void SetRenderDelegate(RenderDelegate* delegate);
  void SetRealTimeRender(bool v);
  void SetNeedRender();
  void SetRenderUI(bool render) { render_ui_ = render;}
  void Present();

  RenderView* GetView() { return render_view_;}
  const char* GetClassName() const override;
  void OnFocus() override;
 private:
  views::NonClientFrameView* CreateNonClientFrameView(views::Widget* widget) override;
  void OnBeforeWidgetInit(views::Widget::InitParams* params,
                          views::Widget* widget) override;
  void OnAfterWidgetInit() override;
  views::Widget* CreateWidget() override;

  // override from views::WidgetObserver
  void OnWidgetDestroying(views::Widget* widget) override;
  void OnWidgetDestroyed(views::Widget* widget) override;
  
  // override from views::View
  bool OnMousePressed(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnMouseMoved(const ui::MouseEvent& event) override;
  bool OnKeyPressed(const ui::KeyEvent& event) override;
  bool OnKeyReleased(const ui::KeyEvent& event) override;
  bool OnMouseWheel(const ui::MouseWheelEvent& event) override;

  bool render_ui_;
  bool need_render_;
  bool realtime_render_;
  RenderLoop* message_loop_;
  RenderView* render_view_;
  RenderDelegate* delegate_;
  std::unique_ptr<SwapchainContext> render_context_;
  SampleDesc sample_desc_;
  DISALLOW_COPY_AND_ASSIGN(RenderWindow);
};
}  // namespace nelf
