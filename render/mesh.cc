#include "azer/render/mesh.h"

#include "base/logging.h"
#include "azer/render/renderer.h"
#include "azer/render/render_closure.h"
#include "azer/render/render_system.h"

namespace azer {
Mesh::Mesh() {
}

Mesh::Mesh(EffectAdapterContext* context)
    : EffectParamsProviderContainer(context) {
}

Mesh::~Mesh() {
}

void Mesh::AddRenderClosure(RenderClosurePtr ptr) {
  closure_.push_back(ptr);
}

void Mesh::UpdateProviderParams(const FrameArgs& args) {
  EffectParamsProviderContainer::UpdateProviderParams(args);
  for (auto iter = closure_.begin(); iter != closure_.end(); ++iter) {
    (*iter)->UpdateProviderParams(args);
  }
}

void Mesh::Draw(Renderer* renderer, Effect* effect, PrimitiveTopology primitive) {
  ApplyParams(effect);
  for (auto iter = closure_.begin(); iter != closure_.end(); ++iter) {
    (*iter)->Draw(renderer, effect, primitive);
  }
}

void Mesh::DrawIndex(Renderer* renderer, Effect* effect,
                     PrimitiveTopology primitive) {
  ApplyParams(effect);
  for (auto iter = closure_.begin(); iter != closure_.end(); ++iter) {
    (*iter)->DrawIndex(renderer, effect, primitive);
  }
}

void Mesh::Render(Renderer* renderer, Effect* effect) {
  DrawIndex(renderer, effect, kTriangleList);
}
}  // namespace azer
