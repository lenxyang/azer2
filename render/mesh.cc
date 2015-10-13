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

void Mesh::ResetCommonProvider() {
  for (auto iter = entity_.begin(); iter != entity_.end(); ++iter) {
    iter->common_adapter.clear();
  }
  common_provider_.clear();
}

void Mesh::RemoveProvider(EffectParamsProviderPtr provider) {
  int index = 0;
  for (auto iter = common_provider_.begin(); 
       iter != common_provider_.end(); 
       ++iter, ++index) {
    if (iter->get() == provider) {
      common_provider_.erase(iter);
      break;
    }
  }

  for (auto iter = entity_.begin(); iter != entity_.end(); ++iter) {
    iter->common_adapter.erase(iter->common_adapter.begin() + index);
  }
}

void Mesh::AddCommonProvider(EffectParamsProviderPtr provider) {
  CHECK(context_);
  common_provider_.push_back(provider);
  for (auto iter = entity_.begin(); iter != entity_.end(); ++iter) {
    std::type_index effect_id = typeid(*(iter->effect.get()));
    std::type_index provider_id = typeid(*(provider.get()));
    const EffectParamsAdapter* adapter = 
        context_->LookupAdapter(effect_id, provider_id);
    iter->common_adapter.push_back(adapter);
  }
}

void Mesh::AddEntity(Entity entity) {
  CHECK(entity.common_adapter.empty());
  if (context_) {
    for (auto iter = common_provider_.begin(); 
         iter != common_provider_.end(); 
         ++iter) {
      std::type_index effect_id = typeid(*(entity.effect.get()));
      std::type_index provider_id = typeid(*(iter->get()));
      const EffectParamsAdapter* adapter = 
          context_->LookupAdapter(effect_id, provider_id);
      entity.common_adapter.push_back(adapter);
    }
  }
  entity_.push_back(entity);
  // lookup provider
}

void Mesh::Update(const FrameArgs& args) {
  for (auto iter = common_provider_.begin(); 
       iter != common_provider_.end(); 
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
  if (entity->adapter)
    entity->adapter->Apply(effect, entity->provider.get());
  for (int32 i = 0; i < static_cast<int32>(common_provider_.size()); ++i) {
    EffectParamsProvider* provider = common_provider_[i].get();
    const EffectParamsAdapter* adapter = entity->common_adapter[i];
    if (adapter) {
      adapter->Apply(effect, provider);
    }
  }
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
