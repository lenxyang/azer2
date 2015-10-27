#include "azer/render/render_closure.h"

#include "azer/render/renderer.h"

namespace azer {
Entity::Entity() {
}

Entity::Entity() {
}

void Entity::Draw(Renderer* renderer, Effect* effect,
                         PrimitiveTopology primitive) {
  vb_->Use(renderer);
  renderer->SetPrimitiveTopology(primitive);
  renderer->Draw(0, vb_->vertex_num());
}

void Entity::DrawIndex(Renderer* renderer, Effect* effect,
                              PrimitiveTopology primitive) {
  vb_->Use(renderer);
  ib_->Use(renderer);
  renderer->SetPrimitiveTopology(primitive);
  renderer->DrawIndex(ib_->indices_num(), 0, 0);
}

void Entity::Render(Renderer* renderer, Effect* effect) {
  if (ib_.get()) {
    DrawIndex(renderer, kTriangleList);
  } else {
    Draw(renderer, kTriangleList);
  }
}

void Entity::SetVertexBuffer(VertexBufferPtr vb) {
  vb_ = vb;
}

void Entity::SetIndicesBuffer(IndicesBufferPtr ib) {
  ib_ = ib;
}
}  // namespace azer
