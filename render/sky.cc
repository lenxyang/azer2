#include "azer/render/sky.h"

namespace azer {
Sky::Sky() {
}
TexturePtr Sky::GetTexture() {
  return texture_;
}
}  // namespace azer
