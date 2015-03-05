#include "azer/render/frame_data.h"

#include "base/time/time.h"

namespace azer {

FrameData::FrameData() 
    : which_(0)
    , frame_cnt_(0)
    , extra_(NULL)
    , total_seconds_(0.0)
    , recent_seconds_(0.0)
    , max_frame_stored_(30) {
  int cur = which_;
  time_[which_] = ::base::Time::Now();
}

void FrameData::UpdateFrameData() {
  which_ ^= 1;
  int cur = which_;
  int prev = which_ ^ 1;
  time_[cur] = ::base::Time::Now();
  delta_ = time_[cur] - time_[prev];
  frame_cnt_++;
  recent_frames_time_consumed_.push_back(delta_);
  recent_seconds_ += delta_.InSecondsF();
  total_seconds_ += delta_.InSecondsF();
  if (recent_frames_time_consumed_.size() > max_frame_stored_) {
    recent_seconds_ -= recent_frames_time_consumed_.front().InSecondsF();
    recent_frames_time_consumed_.pop_front();
  }
}

float FrameData::recent_average_fps() const {
  return recent_frames_time_consumed_.size() / recent_seconds_;
}

float FrameData::total_average_fps() const {
  return frame_cnt_ / total_seconds_;
}
}  // namespace azer
