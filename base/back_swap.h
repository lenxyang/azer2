#pragma once

#include "base/basictypes.h"

namespace azer {

/**
 * some where
 *
 */
template <class T>
class BackSwapChain {
 public:
  BackSwapChain(T back, T front)
      : which_(0) {
    obj_[0] = front;
    obj_[1] = back;
  }

  T& front() { return obj_[which_];}
  T& back() { return obj_[which_ ^ 1];}

  const T& front() const { return obj_[which_];}
  const T& back() const { return obj_[which_ ^ 1];}

  void swap() { which_ ^= 1;}
 private:
  T obj_[2];
  int32_t which_;
  DISALLOW_COPY_AND_ASSIGN(BackSwapChain);
};
}  // namespace azer
