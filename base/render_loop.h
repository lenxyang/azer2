#pragma once

#include <atomic>

#include "base/time/time.h"
#include "base/basictypes.h"
#include "azer/base/export.h"
#include "base/memory/ref_counted.h"

namespace base {
class MessageLoop;
}

namespace azer {

class RenderSystem;

class AZER_EXPORT RenderLoop : public ::base::RefCounted<RenderLoop> {
 public:
  class Delegate {
   public:
    virtual bool Initialize(RenderLoop* renderer) = 0;
    virtual void OnUpdate(const ::base::Time& Time,
                          const ::base::TimeDelta& delta) = 0;
    virtual void OnRender(const ::base::Time& Time,
                          const ::base::TimeDelta& delta) = 0;
  };

  explicit RenderLoop(Delegate* delegate);
  virtual ~RenderLoop();

  RenderSystem* GetRenderSystem() { return render_system_;}
  int64 GetFrameCount() const { return frame_count_;}

  // run renderloop
  // must has a MessageLoopForUI in current thread
  bool Run();

  // quit the frame
  void Quit();
 private:
  bool Init();
  void RenderTask();
  void PostTask();

  Delegate* delegate_;
  RenderSystem* render_system_;
  ::base::MessageLoop* message_loop_;
  uint32 which_;
  ::base::Time time_[2];
  int64 frame_count_;
  std::atomic<bool> stop_;
  DISALLOW_COPY_AND_ASSIGN(RenderLoop);
};
}  // namespace azer
