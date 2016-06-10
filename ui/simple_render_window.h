#pragma once

#include "ui/gfx/geometry/rect.h"
#include "ui/views/widget/widget.h"
#include "azer/render/camera.h"
#include "azer/render/frame_args.h"
#include "azer/ui/window.h"
#include "azer/ui/render_delegate.h"

namespace azer {

class FrameArgs;
class Renderer;
class RenderLoop;
class WindowContext;

class SimpleRenderWindow : public Window,
                           public RenderDelegate,
                           public views::WidgetObserver {
 public:
  SimpleRenderWindow(const gfx::Rect& init_bounds, WindowContext* context);
  ~SimpleRenderWindow();

  Camera* mutable_camera() { return &camera_;}
  const Camera& camera() const { return camera_;}
  RenderLoop* GetRenderLoop() { return render_loop_.get();}
  void SetClearColor(const Vector4& color) { clear_color_ = color;}
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
 private:
  void InitRenderView();
  virtual void OnAfterWidgetInit() override;
  RenderView* render_view_;
  scoped_refptr<RenderLoop> render_loop_;
  Camera camera_;
  Vector4 clear_color_;
  SampleDesc sample_desc_;
  DISALLOW_COPY_AND_ASSIGN(SimpleRenderWindow);
};
}  // namespace azer
