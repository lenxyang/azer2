#pragma once

#include "azer/render/texture.h"
#include "azer/render/renderable_object.h"

namespace azer {
class AZER_EXPORT Sky : public RenderableObject {
 public:
  TexturePtr GetTexture() { return texture_;}
 protected:
  TexturePtr texture_;
  DISALLOW_COPY_AND_ASSIGN(Sky);
};
}  // namespace
