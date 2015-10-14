#include "azer/render/mesh.h"

#include "base/logging.h"
#include "azer/render/renderer.h"
#include "azer/render/render_closure.h"
#include "azer/render/render_system.h"

namespace azer {
Mesh::Mesh(EffectAdapterContext* context)
    : EffectParamsProviderContainer(context) {
}

Mesh::~Mesh() {
}

void Mesh::UpdateParams(const FrameArgs& args) {
  EffectParamsProviderContainer::UpdateParams(args);
  for (auto iter = closure_.begin(); iter != closure_.end(); ++iter) {
    (*iter)->UpdateParams(args);
  }
}

void Mesh::Draw(Renderer* renderer, Effect* effect, PrimitiveTopology primitive) {
  for (auto iter = closure_.begin(); iter != closure_.end(); ++iter) {
    (*iter)->Draw(renderer, effect, primitive);
  }
}

void Mesh::DrawIndex(Renderer* renderer, Effect* effect,
                     PrimitiveTopology primitive) {
  for (auto iter = closure_.begin(); iter != closure_.end(); ++iter) {
    (*iter)->DrawIndex(renderer, effect, primitive);
  }
}

}  // namespace azer
