#pragma once

#include <string>
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {
class AZER_EXPORT Material : public ::base::RefCounted<Material> {
 public:
  explicit Material(int32 id);
  virtual ~Material() {}

  int32 id() const { return id_;}
 private:
  int32 id_;
  DISALLOW_COPY_AND_ASSIGN(Material);
};

typedef scoped_refptr<Material> MaterialPtr;
}  // namespace azer
