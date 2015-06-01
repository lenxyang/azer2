#pragma once

#include <string>
#include "base/memory/ref_counted.h"

namespace azer {
class Light : public ::base::RefCounted<Light> {
 public:
  explicit Light(int32 id);

  int32 id() const { return id_;}
 private:
  int32 id_;
  DISALLOW_COPY_AND_ASSIGN(Light);
};

typedef scoped_refptr<Light> LightPtr;
}  // namespace azer
