#include "azer/render/mesh.h"

#include "azer/render/render_system.h"

namespace azer {

Mesh::Mesh() {
}

Mesh::~Mesh() {
}

void Mesh::Update(const FrameArgs& args) {
  for (auto iter = provider_.begin(); iter != provider_.end(); ++iter) {
    (*iter)->UpdateParams(args);
  }
}

void Mesh::Draw(Renderer* renderer, PrimitiveTopology primitive) {
  for (int32 i = 0; i < static_cast<int32>(entity_.size()); ++i) {
    ApplyEffectParams(i, renderer);
    Entity& entity = entity_[i];
    renderer->Draw(entity.vb.get(), primitive);
  }
}

void Mesh::ApplyEffectParams(int32 index, Renderer* renderer) {
  Group& group = group_[index];
  EffectPtr effect = group.effect;
  DCHECK(group.provider.size() == group.adapter.size());
  for (int32 i = 0; i < static_cast<int32>(group.provider.size()); ++i) {
    EffectParamsProviderPtr provider = provider_[group.provider[i]];
    EffectParamsAdapter* adapter = adapter_[group.adapter[i]];
    adapter->Apply(effect.get(), provider);
  }
  effect->Use(renderer);
}

void Mesh::DrawIndex(Renderer* renderer, PrimitiveTopology primitive) {
  for (int32 i = 0; i < static_cast<int32>(entity_.size()); ++i) {
    ApplyEffectParams(i, renderer);
    Entity& entity = entity_[i];
    renderer->DrawIndex(entity.vb.get(), entity.ib.get(), primitive);
  }
}

void Mesh::AddEntityData(EntityData* data) {
  RenderSystem* rs = RenderSystem::Current();
  EntityBuffer entity_buf;
  entity_buf.vb = rs->CreateVertexBuffer(VertexBuffer::Options(), data->vdata.get());
  entity_buf.ib = rs->CreateIndicesBuffer(IndicesBuffer::Options(), data->idata.get());
  entity_buf.effect = data->effect;
  entity_buf.provider = data->provider;
  entity_buf.adapter = data->adapter;
  return AddEntityBuffer(&entity_buf);
}

void Mesh::AddEntityBuffer(EntityBuffer* data) {
  int32 group_index = group_.size();
  group_.push_back(Group());
  Group& group = group_.back();
  group.effect = data->effect;
  for (int i = 0; i < data->provider.size(); ++i) {
    int provider_index = LookupProviderOrInsert(data->provider[i].get());
    int adapter_index = LookupAdapterOrInsert(data->adapter[i]);
    group.provider.push_back(provider_index);
    group.adapter.push_back(adapter_index);
  }
  
  entity_.push_back(Entity());
  Entity& entity = entity_.back();
  entity.group_index = group_index;
  entity.vb = data->vb;
  entity.ib = data->ib;
}

int32 Mesh::LookupAdapterOrInsert(EffectParamsAdapter* adapter) {
  int32 index = LookupAdapter(adapter);
  if (index == -1) {
    adapter_.push_back(adapter);
    return adapter_.size() - 1;
  } else {
    return index;
  }
}

int32 Mesh::LookupProviderOrInsert(EffectParamsProvider* provider) {
  int32 index = LookupProvider(provider);
  if (index == -1) {
    provider_.push_back(provider);
    return provider_.size() - 1;
  } else {
    return index;
  }
}

int32 Mesh::LookupAdapter(EffectParamsAdapter* adapter) {
  for (int32 i = 0; i < static_cast<int32>(adapter_.size()); ++i) {
    if (adapter_[i] == adapter)
      return i;
  }

  return -1;
}

int32 Mesh::LookupProvider(EffectParamsProvider* provider) {
  for (int32 i = 0; i < static_cast<int32>(provider_.size()); ++i) {
    if (provider_[i].get() == provider)
      return i;
  }

  return -1;
}
}  // namespace azer
