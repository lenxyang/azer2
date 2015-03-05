#include "azer/render/frame_data.h"

#include "base/time/time.h"

namespace azer {

FrameData::FrameData() 
    : frame_cnt(0)
    , extra(NULL)
    , total_seconds(0.0)
    , recent_seconds(0.0)
    , max_frame_stored(30) {
  prev_frame = ::base::Time::Now();
}

void FrameData::UpdateFrameData() {
  prev_frame = current_frame;
  current_frame = ::base::Time::Now();
  time_delta = current_frame - prev_frame;
  frame_cnt++;
  recent_frames_time_consumed.push_back(time_delta);
  recent_seconds += time_delta.InSecondsF();
  total_seconds += time_delta.InSecondsF();
  if (recent_frames_time_consumed.size() > max_frame_stored) {
    recent_seconds -= recent_frames_time_consumed.front().InSecondsF();
    recent_frames_time_consumed.pop_front();
  }
}

float FrameData::recent_average_fps() const {
  return recent_frames_time_consumed.size() / recent_seconds;
}

float FrameData::total_average_fps() const {
  return frame_cnt / total_seconds;
}
}  // namespace azer
