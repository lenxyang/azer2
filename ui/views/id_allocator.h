#pragma once

#include <atomic>
#include "base/basictypes.h"
#include "azer/ui/views/views_export.h"

namespace views {

class VIEWS_EXPORT ViewsIDAllocator {
 public:
  ViewsIDAllocator() : allocated_id_(0) {}

  static ViewsIDAllocator* Pointer();

  int64 allocate_id() { return allocated_id_++;}
 protected:
  std::atomic<int64> allocated_id_;
  DISALLOW_COPY_AND_ASSIGN(ViewsIDAllocator);
};

}  // namespace views
