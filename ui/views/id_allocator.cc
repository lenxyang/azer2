#include "azer/ui/views/id_allocator.h"

#include "base/lazy_instance.h"

namespace views {
::base::LazyInstance<ViewsIDAllocator> id_allocator_ = LAZY_INSTANCE_INITIALIZER;

ViewsIDAllocator* ViewsIDAllocator::Pointer() {
  return id_allocator_.Pointer();
}
}  // namespace views
