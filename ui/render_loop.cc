#include "azer/ui/render_loop.h"

#include "base/logging.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/run_loop.h"
#include "azer/render/render_system.h"
#include "azer/ui/widget_renderer_context.h"

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

RenderLoop::RenderLoop(Delegate* delegate, views::Widget* widget, int32 max_fps)
    : delegate_(delegate)
    , render_system_(NULL)
    , message_loop_(NULL)
    , stopping_(false)
    , kMaxFPS(max_fps) {
  if (kMaxFPS > 0) {
    double time = 1.0 / (double)kMaxFPS;
    expect_frame_consumed_ = ::base::TimeDelta::FromSecondsD(time);
  }
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
  
  if (!delegate_->Initialize(this)) {
    return false;
  }

  delegate_->OnUpdate(&frame_data_);
  return true;
}

void RenderLoop::PostTask(const ::base::TimeDelta& prev_frame_delta) {
  DCHECK(NULL != delegate_);
  DCHECK(NULL != message_loop_);
  DCHECK(message_loop_ == ::base::MessageLoop::current());
  if (stopping_) {
    return;
  }

  ::base::Closure closure = ::base::Bind(&RenderLoop::RenderTask, this);
  ::base::MessageLoop* cur = ::base::MessageLoop::current();
  
  if (kMaxFPS > 0) {
    ::base::TimeDelta delay(expect_frame_consumed_ - prev_frame_delta);
    ::base::MessageLoop::ScopedNestableTaskAllower
          allow(::base::MessageLoop::current());
    if (delay.InSecondsF() > 0) {
      message_loop_->PostDelayedTask(FROM_HERE, closure, delay);
    } else {
      message_loop_->PostTask(FROM_HERE, closure);
    }
  } else {
    ::base::TimeDelta delay(::base::TimeDelta::FromSecondsD(0.0002));
    message_loop_->PostDelayedTask(FROM_HERE, closure, delay);
  }
}

void RenderLoop::RenderTask() {
  if (stopping_) {
    return;
  }

  DCHECK(NULL != delegate_);
  
  frame_data_.Update();
  delegate_->OnRender(&frame_data_);

  widget_context_->RenderUI();
  delegate_->OnUpdate(&frame_data_);

  widget_context_->Present();
  PostTask(frame_data_.delta());
}

bool RenderLoop::Run() {
  ::base::MessageLoop* cur = ::base::MessageLoop::current();
  DCHECK(cur->type() == ::base::MessageLoop::TYPE_UI);
  if (!Init()) {
    return false;
  }

  delegate_->widget_context_ = widget_context_.get();
  DCHECK(cur == message_loop_);
  PostTask(expect_frame_consumed_);
  base::RunLoop().Run();
  return true;
}

void RenderLoop::Quit() {
  DCHECK_EQ(stopping_, false);
  stopping_ = true;
}
}  // namespace azer
