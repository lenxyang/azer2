#pragma once

#include "ui/gfx/geometry/rect.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_observer.h"
#include "azer/render/camera.h"
#include "azer/render/frame_args.h"
#include "azer/ui/window.h"
#include "azer/ui/sample_mainframe.h"
#include "azer/ui/render_delegate.h"

namespace azer {

class FrameArgs;
class FPSPanel;
class Renderer;
class RenderLoop;
class WindowContext;

class SampleMainframe : public Window, public RenderDelegate,
                        public views::WidgetObserver {
 public:
  SampleMainframe(const gfx::Rect& init_bounds, WindowContext* context);
  ~SampleMainframe();

  Camera* mutable_camera() { return &camera_;}
  const Camera& camera() const { return camera_;}
  RenderLoop* GetRenderLoop() { return render_loop_.get();}
  void SetSampleDesc(const SampleDesc& desc) { sample_desc_ = desc;}
 protected:
  virtual void OnInit() {}
  virtual void OnUpdateFrame(const FrameArgs& args) = 0;
  virtual void OnRenderBegin(const FrameArgs&, Renderer*) {}
  virtual void OnRenderFrame(const FrameArgs& args, Renderer* renderer) = 0;
  virtual void OnRenderEnd(const FrameArgs&, Renderer*) {}
 protected:
  // override from RenderDelegate
  bool Initialize() override;
  void OnUpdate(const FrameArgs& args) override;
  void OnRender(const FrameArgs& args) override;

  // override from views::WidgetObserver
  void OnWidgetBoundsChanged(views::Widget* widget, 
                             const gfx::Rect& new_bounds) override;
  void OnWidgetDestroying(views::Widget* widget) override;

  // override from Window, views::WidgetDelegate
  void WindowClosing() override;
 private:
  void InitRenderView();
  virtual void OnAfterWidgetInit() override;
  RenderView* render_view_;
  scoped_refptr<RenderLoop> render_loop_;
  Camera camera_;
  SampleDesc sample_desc_;
  FPSPanel* fpspanel_;
  DISALLOW_COPY_AND_ASSIGN(SampleMainframe);
};
}  // namespace azer
