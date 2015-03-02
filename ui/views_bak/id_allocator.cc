#include "azer/ui/views/id_allocator.h"

#include "base/lazy_instance.h"

namespace views {
::base::LazyInstance<ViewsIDAllocator> id_allocator_ = LAZY_INSTANCE_INITIALIZER;

ViewsIDAllocator* ViewsIDAllocator::Pointer() {
  static ViewsIDAllocator allocator;
  return &allocator;
}
}  // namespace views
