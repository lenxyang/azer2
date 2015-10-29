#include "azer/render/renderer.h"

#include "azer/render/effect.h"

namespace azer {

void Renderer::UseEffect(Effect* effect) {
  effect->Apply(this);
}
}  // namespace azer
