#pragma once

#include <atomic>

#include "base/time/time.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

#include "azer/render/frame_args.h"
#include "azer/render/render_system.h"
#include "azer/render/renderer.h"
#include "azer/render/swap_chain.h"

namespace base {
class MessageLoop;
}

namespace views {
class Widget;
}  // namespace views

namespace azer {
class FrameArgs;
class RenderSystem;
class WidgetRendererContext;

class AZER_EXPORT RenderLoop : public ::base::RefCounted<RenderLoop> {
 public:
  class AZER_EXPORT Delegate {
   public:
    Delegate() {}
    virtual ~Delegate() {}
    virtual bool Initialize(RenderLoop* renderer) = 0;
    virtual void OnUpdate(FrameArgs* frame_data) = 0;
    virtual void OnRender(FrameArgs* frame_data) = 0;

    views::Widget* widget();
    azer::SwapChainPtr& GetSwapChain();
    azer::RendererPtr& GetRenderer();
   private:
    WidgetRendererContext* widget_context_;

    friend class RenderLoop;
    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  explicit RenderLoop(Delegate* delegate, views::Widget* widget);
  virtual ~RenderLoop();

  azer::RenderSystem* GetRenderSystem() { return render_system_;}
  int64 GetFrameCount() const;

  // run renderloop
  // must has a MessageLoopForUI in current thread
  bool Run();

  // quit the frame
  void Quit();
 private:
  bool Init();
  void RenderTask();
  void PostTask(const ::base::TimeDelta& delta);

  scoped_ptr<WidgetRendererContext> widget_context_;
  Delegate* delegate_;
  azer::RenderSystem* render_system_;
  ::base::MessageLoop* message_loop_;
  ::base::TimeDelta average_frame_consumed_;
  FrameArgs frame_data_;
  std::atomic<bool> stopping_;
  DISALLOW_COPY_AND_ASSIGN(RenderLoop);
};

}  // namespace azer
