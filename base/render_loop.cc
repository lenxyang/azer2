#include "azer/base/render_loop.h"

#include "base/logging.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/run_loop.h"
#include "azer/render/render_system.h"

namespace azer {

RenderLoop::RenderLoop(Delegate* delegate)
    : delegate_(delegate)
    , render_system_(NULL)
    , message_loop_(NULL)
    , which_(0)
    , frame_count_(0) 
    , stop_(false) {
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
  PostTask();
  return true;
}

void RenderLoop::PostTask() {
  DCHECK(NULL != delegate_);
  DCHECK(NULL != message_loop_);
  DCHECK(message_loop_ == ::base::MessageLoop::current());
  ::base::Closure closure = ::base::Bind(&RenderLoop::RenderTask, this);
  ::base::MessageLoop* cur = ::base::MessageLoop::current();
  message_loop_->PostTask(FROM_HERE, closure);
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

bool RenderLoop::Run() {
  ::base::MessageLoop* cur = ::base::MessageLoop::current();
  DCHECK(cur->type() == ::base::MessageLoop::TYPE_UI);
  if (!Init()) {
    return false;
  }

  DCHECK(cur == message_loop_);
  while (!stop_.load()) {
    base::RunLoop().RunUntilIdle();
    PostTask();
    render_system_->Present();
  }
  return true;
}

void RenderLoop::Quit() {
  DCHECK_EQ(stop_, false);
  stop_ = false;
}

}  // namespace azer
