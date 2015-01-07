#pragma once

#include "base/basictypes.h"
#inlcude "azer/base/export.h"
#include "azer/base/resource.h"

namespace azer {
namespace resource {

class Context;

class AZER_EXPORT TextureResourceLoader : public ResourceLoader {
 public:
  virtual Resource* Load(const ::base::Value& value) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(TextureResourceLoader);
};
}  // namespace resource
}  // namespace azer
