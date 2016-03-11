#include "azer/render/entity.h"

#include "base/logging.h"
#include "azer/effect/effect_params_adapter.h"
#include "azer/render/bounding_volumn.h"
#include "azer/render/render_system.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
Subset::Subset()
    : vertex_base(0),
      vertex_count(0),
      index_base(0),
      index_count(0),
      primitive(kTriangleList) {
  InitMinAndVMax(&vmin, &vmax);
}

Subset::Subset(int32 vbase, int32 vcount, int32 ibase, int32 icount) 
    : vertex_base(vbase),
      vertex_count(vcount),
      index_base(ibase),
      index_count(icount),
      primitive(kTriangleList) {
  InitMinAndVMax(&vmin, &vmax);
}

// class Entity
Entity::Entity(VertexDesc* desc)
    : primitive_(kTriangleList) {
  InitMinAndVMax(&vmin_, &vmax_);
  RenderSystem* rs = RenderSystem::Current();
  vbg_ = rs->CreateVertexBufferGroup(desc);
}

Entity::Entity(VertexBuffer* vb)
    : primitive_(kTriangleList) {
  InitMinAndVMax(&vmin_, &vmax_);
  RenderSystem* rs = RenderSystem::Current();
  vbg_ = rs->CreateVertexBufferGroup(vb->vertex_desc());
  SetVertexBuffer(vb, 0);
}

Entity::Entity(VertexBufferGroup* vbg)
    : primitive_(kTriangleList) {
  InitMinAndVMax(&vmin_, &vmax_);
  vbg_ = vbg;
}

Entity::Entity(const EntityData& data) {
  InitMinAndVMax(&vmin_, &vmax_);
}

Entity::Entity(VertexBuffer* vb, IndicesBuffer* ib)
    : primitive_(kTriangleList) {
  InitMinAndVMax(&vmin_, &vmax_);
  RenderSystem* rs = RenderSystem::Current();
  vbg_ = rs->CreateVertexBufferGroup(vb->vertex_desc());
  SetVertexBuffer(vb, 0);
  ib_ = ib;
}

Entity::Entity(VertexBufferGroup* vbg, IndicesBuffer* ib)
    : primitive_(kTriangleList) {
  InitMinAndVMax(&vmin_, &vmax_);
  vbg_ = vbg;
  ib_ = ib;
}

Entity::~Entity() {}

VertexBuffer* Entity::vertex_buffer_at(int32 index) { 
  return vbg_->vertex_buffer_at(index);
}

void Entity::AddSubset(const Subset& subset) {
  UpdateVMinAndVMax(subset.vmin, &vmin_, &vmax_);
  UpdateVMinAndVMax(subset.vmax, &vmin_, &vmax_);
  subset_.push_back(subset);
}

void Entity::RemoveSubset(int32 index) {
  DCHECK_LT(index, static_cast<int32>(subset_.size()));
  subset_.erase(subset_.begin() + index);
}

int32 Entity::subset_count() const {
  return static_cast<int32>(subset_.size());
}

const Subset& Entity::subset_at(int32 index) const {
  DCHECK_LT(index, static_cast<int32>(subset_.size()));
  return subset_[index];
}

void Entity::SetVertexBuffer(VertexBuffer* vb, int32 index) {
  vbg_->add_vertex_buffer_at(vb, index);
}

void Entity::SetIndicesBuffer(IndicesBuffer* ib) {
  ib_ = ib;
}

const VertexDesc* Entity::vertex_desc() const {
  return vbg_->vertex_desc();
}

EntityPtr Entity::DeepCopy() {
  NOTIMPLEMENTED();
  return EntityPtr();
}

void Entity::UpdateBounds() {
  InitMinAndVMax(&vmin_, &vmax_);
  for (auto iter = subset_.begin(); iter != subset_.end(); ++iter) {
    UpdateVMinAndVMax(iter->vmin, &vmin_, &vmax_);
    UpdateVMinAndVMax(iter->vmax, &vmin_, &vmax_);
  }
}

void Entity::DrawSubset(Renderer* renderer, const Subset& subset) const {
  DCHECK_EQ(subset.index_count, 0);
  renderer->Draw(subset.vertex_count, subset.vertex_base);
}

void Entity::DrawIndexSubset(Renderer* renderer, const Subset& subset)  const {
  DCHECK_GT(subset.index_count, 0);
  DCHECK(ib_.get() && ib_->indices_count() > 0);
  renderer->DrawIndex(subset.index_count, subset.index_base, subset.vertex_base);
}

void Entity::Draw(Renderer* renderer) const {
  DCHECK(vbg_.get() && vbg_->validate());
  renderer->BindVertexBufferGroup(vbg_.get());
  renderer->BindIndicesBuffer(ib_.get());
  for (auto iter = subset_.begin(); iter != subset_.end(); ++iter) {
    if (iter->primitive != primitive) {
      primitive = iter->primitive;
      renderer->SetPrimitiveTopology(primitive);
    }
    if (iter->index_count > 0) {
      DrawIndexSubset(renderer, *iter);
    } else {
      DrawSubset(renderer, *iter);
    }
  }
}
// class EntityVec
EntityVec::EntityVec() {}

Entity* EntityVec::AddEntity(Entity* ptr) {
  vec_.push_back(ptr);
  UpdateVMinAndVMax(ptr->vmin(), &vmin_, &vmax_);
  UpdateVMinAndVMax(ptr->vmax(), &vmin_, &vmax_);
  return ptr;
}

void EntityVec::RemoveEntityAt(int32 index) {
  DCHECK(index < static_cast<int32>(vec_.size()));
  vec_.erase(vec_.begin() + index);
  UpdateMinAndMax();
}

EntityVecPtr EntityVec::DeepCopy() {
  EntityVecPtr ptr = new EntityVec;
  for (uint32 i = 0; i < vec_.size(); ++i) {
    ptr->AddEntity(vec_[i]->DeepCopy());
  }
  return ptr;
}

void EntityVec::UpdateMinAndMax() {
  for (auto iter = vec_.begin(); iter != vec_.end(); ++iter) {
    Entity* entity = iter->get();
    UpdateVMinAndVMax(entity->vmin(), &vmin_, &vmax_);
    UpdateVMinAndVMax(entity->vmax(), &vmin_, &vmax_);
  }
}
}  // namespace azer
