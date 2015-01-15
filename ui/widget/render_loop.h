#pragma once

#include "base/time/time.h"
#include "base/basictypes.h"
#include "azer/base/export.h"

namespace base {
class MessageLoop;
}

namespace azer {

class RenderSystem;

namespace widget {

class RenderLoop;
class WidgetTreeHost;

class AZER_EXPORT RenderLoopDelegate {
 public:
  virtual bool Initialize(RenderLoop* renderer) = 0;
  virtual void OnUpdate(const ::base::Time& Time,
                        const ::base::TimeDelta& delta) = 0;
  virtual void OnRender(const ::base::Time& Time,
                        const ::base::TimeDelta& delta) = 0;
};

class AZER_EXPORT RenderLoop {
 public:
  explicit RenderLoop(RenderLoopDelegate* delegate);
  virtual ~RenderLoop();

  RenderSystem* GetRenderSystem() { return render_system_;}
  int64 GetFrameCount() const { return frame_count_;}

  // run renderloop
  // must has a MessageLoopForUI in current thread
  bool Run(WidgetTreeHost* host);
  bool RunOneFrame();

 private:
  bool Init();
  void RenderTask();

  RenderLoopDelegate* delegate_;
  RenderSystem* render_system_;
  ::base::MessageLoop* message_loop_;
  uint32 which_;
  ::base::Time time_[2];
  int64 frame_count_;
  DISALLOW_COPY_AND_ASSIGN(RenderLoop);
};

}  // namespace widget
}  // namespace azer


