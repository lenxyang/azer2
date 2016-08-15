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

Subset::Subset(int32_t vbase, int32_t vcount, int32_t ibase, int32_t icount) 
    : vertex_base(vbase),
      vertex_count(vcount),
      index_base(ibase),
      index_count(icount),
      primitive(kTriangleList) {
  InitMinAndVMax(&vmin, &vmax);
}

Subset::Subset(int32_t vbase, int32_t vcount, int32_t ibase, int32_t icount, 
               PrimitiveTopology type)
    : vertex_base(vbase),
      vertex_count(vcount),
      index_base(ibase),
      index_count(icount),
      primitive(type) {
  InitMinAndVMax(&vmin, &vmax);
}

EntityData::EntityData(VertexData* vdata)
    : vdata_(vdata) {
}

EntityData::EntityData(VertexData* vdata, IndicesData* idata)
    : vdata_(vdata),
      idata_(idata) {
}
EntityData::EntityData(VertexDesc* desc, int32_t count) {
  vdata_ = new VertexData(desc, count);
}

EntityData::~EntityData() {
}

// class Entity
Entity::Entity(VertexDesc* desc, int slot_count) {
  InitMinAndVMax(&vmin_, &vmax_);
  vbg_ = new VertexBufferGroup(desc, slot_count);
}

Entity::Entity(VertexBuffer* vb) {
  InitMinAndVMax(&vmin_, &vmax_);
  vbg_ = new VertexBufferGroup(vb->vertex_desc(), 1);
  SetVertexBuffer(vb, 0);
}

Entity::Entity(VertexBufferGroup* vbg) {
  InitMinAndVMax(&vmin_, &vmax_);
  vbg_ = vbg;
}

Entity::Entity(EntityData* data) {
  InitMinAndVMax(&vmin_, &vmax_);
  vbg_ = new VertexBufferGroup(data->vdata());
  if (data->idata())
    ib_ = new IndicesBuffer(data->idata());
  subset_ = data->subset();
}

Entity::Entity(VertexBuffer* vb, IndicesBuffer* ib) {
  InitMinAndVMax(&vmin_, &vmax_);
  vbg_ = new VertexBufferGroup(vb->vertex_desc(), 1);
  vbg_->set_vertex_buffer_at(vb, 0);
  ib_ = ib;
}

Entity::Entity(VertexBufferGroup* vbg, IndicesBuffer* ib) {
  InitMinAndVMax(&vmin_, &vmax_);
  vbg_ = vbg;
  ib_ = ib;
}

Entity::~Entity() {
}

VertexBuffer* Entity::vertex_buffer_at(int32_t index) { 
  return vbg_->vertex_buffer_at(index);
}

void Entity::AddSubset(const Subset& subset) {
  UpdateVMinAndVMax(subset.vmin, &vmin_, &vmax_);
  UpdateVMinAndVMax(subset.vmax, &vmin_, &vmax_);
  subset_.push_back(subset);
}

void Entity::RemoveSubset(int32_t index) {
  DCHECK_LT(index, static_cast<int32_t>(subset_.size()));
  subset_.erase(subset_.begin() + index);
}

int32_t Entity::subset_count() const {
  return static_cast<int32_t>(subset_.size());
}

const Subset& Entity::subset_at(int32_t index) const {
  DCHECK_LT(index, static_cast<int32_t>(subset_.size()));
  return subset_[index];
}

void Entity::SetVertexBuffer(VertexBuffer* vb, int32_t index) {
  vbg_->set_vertex_buffer_at(vb, index);
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

void Entity::DrawSub(int32_t index, Renderer* renderer) const {
  DCHECK(vbg_.get() && vbg_->validate());
  renderer->BindVertexBufferGroup(vbg_.get());
  renderer->BindIndicesBuffer(ib_.get());
  const Subset& subset = subset_[index];
  renderer->SetPrimitiveTopology(subset.primitive);
  if (subset.index_count > 0) {
    DrawIndexSubset(renderer, subset);
  } else {
    DrawSubset(renderer, subset);
  }
}

void Entity::Draw(Renderer* renderer) const {
  DCHECK(vbg_.get() && vbg_->validate());
  renderer->BindVertexBufferGroup(vbg_.get());
  renderer->BindIndicesBuffer(ib_.get());
  PrimitiveTopology primitive = subset_.empty() ? kTriangleList : 
      subset_[0].primitive;
  renderer->SetPrimitiveTopology(primitive);
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

void EntityVec::RemoveEntityAt(int32_t index) {
  DCHECK(index < static_cast<int32_t>(vec_.size()));
  vec_.erase(vec_.begin() + index);
  UpdateMinAndMax();
}

EntityVecPtr EntityVec::DeepCopy() {
  EntityVecPtr ptr = new EntityVec;
  for (uint32_t i = 0; i < vec_.size(); ++i) {
    EntityPtr newptr = vec_[i]->DeepCopy();
    ptr->AddEntity(newptr.get());
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
