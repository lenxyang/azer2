#pragma once

#include <deque>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time/time.h"

namespace azer {
struct AZER_EXPORT FrameData {
 public:
  FrameData();

  ::base::Time prev_frame;
  ::base::Time current_frame;
  ::base::Time time_delta;
  int64 frame_cnt;
  void *extra;

  void UpdateFrameData();
  float recent_average_fps() const; 
  float total_average_fps() const; 
 private:
  double total_seconds; 
  double recent_seconds;
  std::deque<::base::TimeDelta> recent_frames_time_consumed;
  const int32 max_frame_stored;
  DISALLOW_COPY_AND_ASSIGN(FrameData);
};



}  // namespace azer
