#include "azer/render/frame_args.h"

#include "base/time/time.h"

namespace azer {

FrameArgs::FrameArgs() 
    : timef_(0.0),
      which_(0),
      frame_cnt_(0),
      frame_id_(0),
      extra_(NULL),
      total_seconds_(0.0),
      recent_seconds_(0.0),
      max_frame_stored_(30) {
  time_[which_] = ::base::Time::Now();
  started_ = time_[which_];
  recent_frames_time_consumed_.push_back(::base::TimeDelta());
}

void FrameArgs::Update() {
  frame_id_++;
  which_ ^= 1;
  int cur = which_;
  int prev = which_ ^ 1;
  time_[cur] = ::base::Time::Now();
  delta_ = time_[cur] - time_[prev];
  ::base::TimeDelta total_delta = time_[cur] - started_;
  timef_ = total_delta.InSecondsF();
  frame_cnt_++;
  recent_frames_time_consumed_.push_back(delta_);
  recent_seconds_ += delta_.InSecondsF();
  total_seconds_ += delta_.InSecondsF();
  if (recent_frames_time_consumed_.size() > max_frame_stored_) {
    recent_seconds_ -= recent_frames_time_consumed_.front().InSecondsF();
    recent_frames_time_consumed_.pop_front();
  }
}

float FrameArgs::recent_average_fps() const {
  return (float)recent_frames_time_consumed_.size() / recent_seconds_;
}

float FrameArgs::total_average_fps() const {
  return (float)frame_cnt_ / total_seconds_;
}
}  // namespace azer
