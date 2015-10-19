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
  vb_->Use(renderer);
  renderer->SetPrimitiveTopology(primitive);
  renderer->Draw(0, vb_->vertex_num());
}

void RenderClosure::DrawIndex(Renderer* renderer, Effect* effect,
                              PrimitiveTopology primitive) {
  ApplyParams(effect);
  effect->Use(renderer);
  vb_->Use(renderer);
  ib_->Use(renderer);
  renderer->SetPrimitiveTopology(primitive);
  renderer->DrawIndex(ib_->indices_num(), 0, 0);
}

void RenderClosure::SetVertexBuffer(VertexBufferPtr vb) {
  vb_ = vb;
}

void RenderClosure::SetIndicesBuffer(IndicesBufferPtr ib) {
  ib_ = ib;
}
}  // namespace azer
