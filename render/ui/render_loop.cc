#include "azer/render/ui/render_loop.h"

#include "base/logging.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/run_loop.h"
#include "azer/render/render_system.h"
#include "azer/render/ui/widget_renderer_context.h"

namespace azer {
views::Widget* RenderLoop::Delegate::widget() {
  return widget_context_->widget();
}

azer::SwapChainPtr& RenderLoop::Delegate::GetSwapChain() {
  return widget_context_->GetSwapChain();
}

azer::RendererPtr& RenderLoop::Delegate::GetRenderer() {
  return widget_context_->GetRenderer();
}

RenderLoop::RenderLoop(Delegate* delegate, views::Widget* widget)
    : delegate_(delegate)
    , render_system_(NULL)
    , message_loop_(NULL)
    , which_(0)
    , frame_count_(0) 
    , stopping_(false) {
  widget_context_.reset(new WidgetRendererContext(widget));
}

RenderLoop::~RenderLoop() {
}

bool RenderLoop::Init() {
  DCHECK(NULL != delegate_);
  message_loop_ = ::base::MessageLoop::current();
  DCHECK(message_loop_->type() == ::base::MessageLoop::TYPE_UI);

  render_system_ = azer::RenderSystem::Current();
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

void RenderLoop::PostTask() {
  DCHECK(NULL != delegate_);
  DCHECK(NULL != message_loop_);
  DCHECK(message_loop_ == ::base::MessageLoop::current());
  if (stopping_) {
    return;
  }

  ::base::Closure closure = ::base::Bind(&RenderLoop::RenderTask, this);
  ::base::MessageLoop* cur = ::base::MessageLoop::current();

  ::base::MessageLoop::ScopedNestableTaskAllower
        allow(::base::MessageLoop::current());
  message_loop_->PostTask(FROM_HERE, closure);
}

void RenderLoop::RenderTask() {
  if (stopping_) {
    return;
  }

  DCHECK(NULL != delegate_);
  which_ ^= 1;
    
  int cur = which_;
  int prev = which_ ^ 1;
  time_[cur] = ::base::Time::Now();
  ::base::TimeDelta delta = time_[cur] - time_[prev];
  delegate_->OnRender(time_[cur], delta);

  widget_context_->RenderUI();
  widget_context_->Present();
  frame_count_++;
  delegate_->OnUpdate(time_[cur], delta);

  PostTask();
}

bool RenderLoop::Run() {
  ::base::MessageLoop* cur = ::base::MessageLoop::current();
  DCHECK(cur->type() == ::base::MessageLoop::TYPE_UI);
  if (!Init()) {
    return false;
  }

  delegate_->widget_context_ = widget_context_.get();
  DCHECK(cur == message_loop_);
  PostTask();
  base::RunLoop().Run();
  return true;
}

void RenderLoop::Quit() {
  DCHECK_EQ(stopping_, false);
  stopping_ = true;
}
}  // namespace azer
