#pragma once

#include "base/memory/ref_counted.h"
#include "azer/render/texture.h"
#include "azer/render/renderable_object.h"

namespace azer {
class AZER_EXPORT Sky : public RenderableObject {
 public:
  Sky();
  TexturePtr GetTexture();
 protected:
  TexturePtr texture_;
  DISALLOW_COPY_AND_ASSIGN(Sky);
};

typedef scoped_refptr<Sky> SkyPtr;
}  // namespace
