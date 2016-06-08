#pragma once

#include <deque>
#include <string>
#include <vector>


#include "base/time/time.h"
#include "azer/base/export.h"

namespace azer {
class AZER_EXPORT FrameArgs {
 public:
  FrameArgs();

  void Update();
  
  int64_t frame_id() const { return frame_id_;}
  float recent_average_fps() const; 
  float total_average_fps() const; 

  double time() const { return timef_;}
  const ::base::TimeDelta& delta() const { return delta_;}
 private:
  uint32_t which_;
  double timef_;
  ::base::Time started_;
  ::base::Time time_[2];
  ::base::TimeDelta delta_;
  int64_t frame_cnt_;
  int64_t frame_id_;
  void *extra_;
  double total_seconds_; 
  double recent_seconds_;
  std::deque<::base::TimeDelta> recent_frames_time_consumed_;
  const int32_t max_frame_stored_;
  DISALLOW_COPY_AND_ASSIGN(FrameArgs);
};



}  // namespace azer
