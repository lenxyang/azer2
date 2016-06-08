#include "azer/ui/render_loop.h"

#include "base/bind.h"
#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "ui/views/widget/widget.h"

#include "azer/ui/render_window.h"
#include "azer/ui/render_view.h"

namespace azer {

RenderLoop::RenderLoop(RenderWindow* window)
    : render_system_(NULL),
      message_loop_(NULL),
      render_window_(window),
      running_(false),
      stop_(false),
      fps_limit_(-1) {
  mainframe->GetWidget()->AddObserver(this);
}

RenderLoop::~RenderLoop() {
}

bool RenderLoop::AddRenderView(RenderView* view) {
  DCHECK(view);
  DCHECK(!running_);
  DCHECK(!Contains(view));
  render_view_.push_back(view);
  return true;
}

bool RenderLoop::RemoveRenderView(RenderView* view) {
  for (auto iter = render_view_.begin(); iter != render_view_.end(); ++iter) {
    if (*iter == view) {
      render_view_.erase(iter);
      return true;
    }
  }

  return false;
}

bool RenderLoop::Contains(RenderView* view) const {
  auto iter = std::find(render_view_.begin(), render_view_.end(), view);
  return iter != render_view_.end();
}

void RenderLoop::AddObserver(RenderLoopObserver* observer) {
  observers_.AddObserver(observer);
}

void RenderLoop::RemoveObserver(RenderLoopObserver* observer) {
  observers_.RemoveObserver(observer);
}

bool RenderLoop::Init() {
  message_loop_ = ::base::MessageLoop::current();
  DCHECK(message_loop_->type() == ::base::MessageLoop::TYPE_UI);
  render_system_ = azer::RenderSystem::Current();
  if (NULL == render_system_) {
    LOG(ERROR) << "RenderSystem not initialized.";
    return false;
  }
  
  return true;
}

void RenderLoop::PostTask(const ::base::TimeDelta& prev_frame_delta) {
  DCHECK(NULL != message_loop_);
  DCHECK(message_loop_ == ::base::MessageLoop::current());
  if (stop_) {
    return;
  }

  ::base::Closure closure = ::base::Bind(&RenderLoop::RenderTask, this);
  if (fps_limit_ > 0) {
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
  if (stop_) {
    return;
  }
  FOR_EACH_OBSERVER(RenderLoopObserver, observers_, OnRenderLoopBegin(this));
  frame_data_.Update();
  
  for (auto iter = render_view_.begin(); iter != render_view_.end(); ++iter) {
    RenderView* view = (*iter);
    view->window()->Render(frame_data_);
  }

  for (auto iter = render_view_.begin(); iter != render_view_.end(); ++iter) {
    RenderView* view = (*iter);
    view->window()->Present();
  }

  PostTask(frame_data_.delta());
  FOR_EACH_OBSERVER(RenderLoopObserver, observers_, OnRenderLoopCompleted(this));
}

bool RenderLoop::Run() {
  running_ = true;
  ::base::MessageLoop* cur = ::base::MessageLoop::current();
  DCHECK(cur->type() == ::base::MessageLoop::TYPE_UI);
  if (!Init()) {
    return false;
  }

  base::RunLoop().RunUntilIdle();
  for (auto iter = render_view_.begin(); iter != render_view_.end(); ++iter) {
    RenderView* view = (*iter);
    view->window()->FirstRender();
  }

  FOR_EACH_OBSERVER(RenderLoopObserver, observers_, OnRenderBegin(this));
  DCHECK(cur == message_loop_);
  PostTask(expect_frame_consumed_);
  base::RunLoop().Run();

  FOR_EACH_OBSERVER(RenderLoopObserver, observers_, OnRenderExited(this));
  running_ = false;
  return true;
}

void RenderLoop::OnWidgetClosing(views::Widget* widget) {
  stop_ = true;
}

void RenderLoop::SetMaxFPS(int fps) {
  fps_limit_ = fps;
  if (fps > 0) {
    double time = 1.0 / (double)fps;
    expect_frame_consumed_ = ::base::TimeDelta::FromSecondsD(time);
  } else {
    expect_frame_consumed_ = ::base::TimeDelta();
  }
}
}  // namespace azer
