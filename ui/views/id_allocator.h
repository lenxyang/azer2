#pragma once

#include "base/basictypes.h"

namespace views {

class ViewsIDAllocator {
 public:
  ViewsIDAllocator() : allocated_id_(0) {}

  static ViewsIDAllocator* Pointer();

  int64 allocate_id() { return allocated_id_++;}
 protected:
  std::atomic<int64> allocated_id_;
  DISALLOW_COPY_AND_ASSIGN(ViewsIDAllocator);
};

}  // namespace views
