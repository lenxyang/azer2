#include "azer/ui/views/cull_set.h"

namespace azer {
namespace views {

CullSet::CullSet() {
}

CullSet::~CullSet() {
}

CullSet::CullSet(scoped_ptr<base::hash_set<intptr_t> > cull_set)
    : cull_set_(cull_set.Pass()) {
}

bool CullSet::ShouldPaint(const View* view) const {
  if (cull_set_)
    return (cull_set_->count(reinterpret_cast<intptr_t>(view)) > 0);

  return true;
}

}  // namespace views
}  // namespace azer
