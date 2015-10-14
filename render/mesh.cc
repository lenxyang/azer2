#include "azer/render/mesh.h"

#include "base/logging.h"
#include "azer/render/render_system.h"

namespace azer {
Mesh::Mesh() 
    : context_(NULL) {
}

Mesh::Mesh(EffectAdapterContext* context)
    : context_(context) {
}

Mesh::~Mesh() {
}

Mesh::Entity* Mesh::entity_at(int32 index) {
  return &entity_.at(index);
}

const Mesh::Entity* Mesh::entity_at(int32 index) const {
  return &entity_.at(index);
}

Mesh::Entity Mesh::RemoveEntityAt(int32 index) {
  CHECK_LT(index, entity_.size());
  Mesh::Entity entity = entity_.at(index);
  entity_.erase(entity_.begin() + index);
  return entity;
}

void Mesh::ResetProvider() {
  provider_.clear();
}

void Mesh::RemoveProvider(EffectParamsProviderPtr provider) {
  int index = 0;
  for (auto iter = provider_.begin(); 
       iter != provider_.end(); 
       ++iter, ++index) {
    if (iter->get() == provider) {
      provider_.erase(iter);
      break;
    }
  }
}

void Mesh::AddProvider(EffectParamsProviderPtr provider) {
  CHECK(context_);
  provider_.push_back(provider);
}

void Mesh::AddEntity(Entity entity) {
  entity_.push_back(entity);
}

void Mesh::Update(const FrameArgs& args) {
  for (auto iter = provider_.begin(); 
       iter != provider_.end(); 
       ++iter) {
    (*iter)->UpdateParams(args);
  }

  for (auto iter = entity_.begin(); 
       iter != entity_.end();
       ++iter) {
    iter->provider->UpdateParams(args);
  }
}

void Mesh::ApplyEffectParams(Entity* entity, Renderer* renderer) {
  Effect* effect = entity->effect.get();
  effect->Use(renderer);
}

void Mesh::DrawIndex(Renderer* renderer, PrimitiveTopology primitive) {
  for (int32 i = 0; i < static_cast<int32>(entity_.size()); ++i) {
    Entity& entity = entity_[i];
    ApplyEffectParams(&entity, renderer);
    renderer->DrawIndex(entity.vb.get(), entity.ib.get(), primitive);
  }
}

void Mesh::Draw(Renderer* renderer, PrimitiveTopology primitive) {
  for (int32 i = 0; i < static_cast<int32>(entity_.size()); ++i) {
    Entity& entity = entity_[i];
    ApplyEffectParams(&entity, renderer);
    renderer->Draw(entity.vb.get(), primitive);
  }
}

}  // namespace azer
