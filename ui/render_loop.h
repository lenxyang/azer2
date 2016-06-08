#pragma once

#include <atomic>

#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop.h"
#include "base/observer_list.h"
#include "base/time/time.h"

#include "ui/views/widget/widget_observer.h"
#include "azer/render/frame_args.h"
#include "azer/render/render_system.h"

namespace azer {

class RenderLoop;
class RenderView;
class RenderWindow;
class RenderLoopObserver;
class WidgetRendererContext;

class RenderLoopObserver {
 public:
  virtual void OnRenderBegin(RenderLoop*) {}
  virtual void OnRenderLoopBegin(RenderLoop*) {}
  virtual void OnRenderLoopCompleted(RenderLoop*) {}
  virtual void OnRenderExited(RenderLoop*) {}
};

class RenderLoop : public views::WidgetObserver,
                   public ::base::RefCounted<RenderLoop> {
 public:
  // the main window's widget
  explicit RenderLoop(RenderWindow* window);
  ~RenderLoop();

  azer::RenderSystem* GetRenderSystem() { return render_system_;}
  int64_t GetFrameCount() const;

  void SetMaxFPS(int fps);
  int max_fps() const { return fps_limit_;}

  bool AddRenderView(RenderView* view);
  bool RemoveRenderView(RenderView* view);
  bool Contains(RenderView* view) const;

  void AddObserver(RenderLoopObserver* observer);
  void RemoveObserver(RenderLoopObserver* observer);

  const azer::FrameArgs& frame_args() const { return frame_data_;}

  // run renderloop
  // must has a RenderLoopForUI in current thread
  bool Run();
 protected:
  void OnWidgetClosing(views::Widget* widget) override;

  bool Init();
  void RenderTask();
  void PostTask(const ::base::TimeDelta& prev_frame_delta);
 private:
  std::vector<RenderView*> render_view_;
  azer::RenderSystem* render_system_;
  ::base::MessageLoop* message_loop_;
  ::base::TimeDelta expect_frame_consumed_;
  RenderWindow* render_window_;
  azer::FrameArgs frame_data_;
  std::atomic<bool> stop_;
  std::atomic<bool> running_;
  ::base::ObserverList<RenderLoopObserver> observers_;
  int fps_limit_;
  DISALLOW_COPY_AND_ASSIGN(RenderLoop);
};
}   // namespace azer
