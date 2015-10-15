#include "azer/render/render_closure.h"

#include "azer/render/renderer.h"

namespace azer {
RenderClosure::RenderClosure() {
}

RenderClosure::RenderClosure(EffectAdapterContext* context)
    : EffectParamsProviderContainer(context) {
}

void RenderClosure::Draw(Renderer* renderer, Effect* effect,
                         PrimitiveTopology primitive) {
  ApplyParams(effect);
  effect->Use(renderer);
  renderer->Draw(vb_.get(), primitive);
}

void RenderClosure::DrawIndex(Renderer* renderer, Effect* effect,
                              PrimitiveTopology primitive) {
  ApplyParams(effect);
  effect->Use(renderer);
  renderer->DrawIndex(vb_.get(), ib_.get(), primitive);
}

void RenderClosure::SetVertexBuffer(VertexBufferPtr vb) {
  vb_ = vb;
}

void RenderClosure::SetIndicesBuffer(IndicesBufferPtr ib) {
  ib_ = ib;
}
}  // namespace azer
