#pragma once

#include <deque>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "azer/base/export.h"

namespace azer {
class AZER_EXPORT FrameArgs {
 public:
  FrameArgs();

  void Update();

  float recent_average_fps() const; 
  float total_average_fps() const; 

  double time() const { return timef_;}
  const ::base::TimeDelta& delta() const { return delta_;}
 private:
  uint32 which_;
  double timef_;
  ::base::Time started_;
  ::base::Time time_[2];
  ::base::TimeDelta delta_;
  int64 frame_cnt_;
  void *extra_;
  double total_seconds_; 
  double recent_seconds_;
  std::deque<::base::TimeDelta> recent_frames_time_consumed_;
  const int32 max_frame_stored_;
  DISALLOW_COPY_AND_ASSIGN(FrameArgs);
};



}  // namespace azer
