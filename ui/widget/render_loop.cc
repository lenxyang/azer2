#include "azer/ui/widget/render_loop.h"

#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/time/time.h"

#include "azer/render/render_system.h"
#include "azer/ui/widget/widget_tree_host.h"

namespace azer {
namespace widget {

RenderLoop::RenderLoop(RenderLoopDelegate* delegate)
    : delegate_(delegate)
    , render_system_(NULL)
    , message_loop_(NULL)
    , which_(0)
    , frame_count_(0) {
}

RenderLoop::~RenderLoop() {
}

bool RenderLoop::Init() {
  DCHECK(NULL != delegate_);
  message_loop_ = ::base::MessageLoop::current();
  DCHECK(message_loop_->type() == ::base::MessageLoop::TYPE_UI);

  render_system_ = RenderSystem::Current();
  if (NULL == render_system_) {
    LOG(ERROR) << "RenderSystem not initialized.";
    return false;
  }

  int cur = which_;
  time_[which_] = ::base::Time::Now();
  if (!delegate_->Initialize(this)) {
    return false;
  }

  delegate_->OnUpdate(time_[cur], ::base::TimeDelta());
  return true;
}

void RenderLoop::RenderTask() {
  DCHECK(NULL != delegate_);
  which_ ^= 1;
    
  int cur = which_;
  int prev = which_ ^ 1;
  time_[cur] = ::base::Time::Now();
  ::base::TimeDelta delta = time_[cur] - time_[prev];
  delegate_->OnRender(time_[cur], delta);
  frame_count_++;
  delegate_->OnUpdate(time_[cur], delta);
}

bool RenderLoop::Run(WidgetTreeHost* host) {
  ::base::MessageLoop* cur = ::base::MessageLoop::current();
  DCHECK(cur->type() == ::base::MessageLoop::TYPE_UI);
  if (!Init()) {
    return false;
  }

  DCHECK(cur == message_loop_);
  while (host->Closed()) {
    base::RunLoop().RunUntilIdle();
    RenderTask();
    render_system_->Present();
  }
  return true;
}

}  // namespace widget
}  // namespace azer
