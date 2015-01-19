#pragma once

#include "base/containers/hash_tables.h"
#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "azer/base/export.h"

namespace azer {
namespace views {

class View;

// A CullSet defines a set of View pointers which have been possibly culled
// from painting or other bounds-checking operations. It wraps a set of
// pointers to views, or NULL if no such set is available.
class AZER_EXPORT CullSet {
 public:
  // Default constructor builds a CullSet that will always return true for
  // ShouldPaint().
  CullSet();
  ~CullSet();

  // Wraps a set of pointers to Views, as might be provided by
  // gfx::RTree::Query(), that intersect the damage rect and therefore need
  // to be painted. CullSet takes ownership of the provided pointer.
  CullSet(scoped_ptr<base::hash_set<intptr_t> > cull_set);

  // Returns true if |view| needs to be painted.
  bool ShouldPaint(const View* view) const;

 private:
  friend class BoundsTreeTestView;

  // The set of Views that collided with the query rectangle provided to the
  // RTree data structure, or NULL if one is not available.
  scoped_ptr<base::hash_set<intptr_t> > cull_set_;

  DISALLOW_COPY_AND_ASSIGN(CullSet);
};

}  // namespace views
}  // namespace azer

