#include "azer/render/mesh.h"

#include "base/logging.h"
#include "azer/render/blending.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/effect_params_adapter.h"

namespace azer {

// class Entity
Entity::Entity(VertexDesc* desc)
    : primitive_(kTriangleList),
      vertex_base_(0),
      vertex_count_(0),
      index_base_(0),
      index_count_(0) {
  RenderSystem* rs = RenderSystem::Current();
  vbg_ = rs->CreateVertexBufferGroup(desc);
}

Entity::Entity(VertexBuffer* vb)
    : primitive_(kTriangleList),
      vertex_base_(0),
      vertex_count_(0),
      index_base_(0),
      index_count_(0) {
  RenderSystem* rs = RenderSystem::Current();
  vbg_ = rs->CreateVertexBufferGroup(vb->vertex_desc());
  SetVertexBuffer(vb, 0);
}

Entity::Entity(VertexBufferGroup* vbg)
    : primitive_(kTriangleList),
      vertex_base_(0),
      vertex_count_(0),
      index_base_(0),
      index_count_(0) {
  vbg_ = vbg;
}

Entity::Entity(VertexBuffer* vb, IndicesBuffer* ib)
    : primitive_(kTriangleList),
      vertex_base_(0),
      vertex_count_(0),
      index_base_(0),
      index_count_(0) {
  RenderSystem* rs = RenderSystem::Current();
  vbg_ = rs->CreateVertexBufferGroup(vb->vertex_desc());
  SetVertexBuffer(vb, 0);
  ib_ = ib;
}

Entity::Entity(VertexBufferGroup* vbg, IndicesBuffer* ib)
    : primitive_(kTriangleList),
      vertex_base_(0),
      vertex_count_(0),
      index_base_(0),
      index_count_(0) {
  vbg_ = vbg;
  ib_ = ib;
}

VertexBuffer* Entity::vertex_buffer_at(int32 index) { 
  return vbg_->vertex_buffer_at(index);
}

/*
Entity::Entity(VertexBuffer* vb, IndicesBuffer* ib)
    : ib_(ib),
      primitive_(kTriangleList) {
  vbg_ = new VertexBufferGroup;
  SetVertexBuffer(vb, 0);
  vbg_->add_vertex_buffer(vb.get());
}
*/

Entity::~Entity() {
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

void Entity::Render(Renderer* renderer) {
  if (ib_.get()) {
    DrawIndex(renderer);
  } else {
    Draw(renderer);
  }
}

void Entity::Draw(Renderer* renderer) {
  DCHECK(vbg_.get() && vbg_->validate());
  renderer->UseVertexBufferGroup(vbg_.get());
  renderer->UseIndicesBuffer(NULL);
  renderer->SetPrimitiveTopology(primitive_type());
  int32 count = (vertex_count_ > 0) ? vertex_count_ : vbg_->vertex_count();
  renderer->Draw(count, vertex_base_);
}

void Entity::DrawIndex(Renderer* renderer) {
  DCHECK(vbg_.get() && vbg_->validate());
  renderer->UseVertexBufferGroup(vbg_.get());
  renderer->UseIndicesBuffer(ib_.get());
  renderer->SetPrimitiveTopology(primitive_type());
  int32 count = (index_count_ > 0) ? index_count_ : ib_->indices_count();
  renderer->DrawIndex(count, vertex_base_, index_base_);
}

// class EntityVec
EntityVec::EntityVec() {
}

void EntityVec::AddEntity(EntityPtr ptr) {
  vec_.push_back(ptr);
  UpdateVMinAndVMax(ptr->vmin(), &vmin_, &vmax_);
  UpdateVMinAndVMax(ptr->vmax(), &vmin_, &vmax_);
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

// MeshPart
MeshPart::MeshPart(Effect* effect)
    : effect_(effect) {
  vecptr_ = new EntityVec;
}

MeshPart::MeshPart(Effect* effect, EntityVecPtr vec) 
    : effect_(effect),
      vecptr_(vec) {
}

MeshPart::MeshPart(const MeshPart& part) {
  *this = part;
}

MeshPart& MeshPart::operator = (const MeshPart& part) {
  effect_ = part.effect_;
  vecptr_ = part.vecptr_;
  return *this;
}

void MeshPart::Render(Renderer* renderer) {
  DCHECK(context_ || vector_.size() == 0u);
  if (!blending_.get()) {
    RenderPart(renderer);
  } else {
    scoped_ptr<ScopedResetBlending> autoblending_;
    autoblending_.reset(new ScopedResetBlending(renderer));
    renderer->UseBlending(blending_.get(), 0);
    RenderPart(renderer);
  }
}

void MeshPart::RenderPart(Renderer* renderer) {
  DCHECK(effect_.get()) << "MeshPart Entity cannot be NULL";
  ApplyParams(effect_.get());
  renderer->UseEffect(effect_.get());
  for (int32 i = 0; i < vecptr_->entity_count(); ++i) {
    Entity* entity = vecptr_->entity_at(i);
    entity->Render(renderer);
  }
}

// class Mesh
Mesh::Mesh() 
    : blending_count_(0) {
}

Mesh::Mesh(EffectAdapterContext* context)
    : EffectParamsProviderContainer(context),
      blending_count_(0) {
}

Mesh::~Mesh() {
}

void Mesh::AddMeshPart(MeshPart* ptr) {
  ptr->SetEffectAdapterContext(context_);
  part_.push_back(MeshPartPtr(ptr));
  UpdateVMinAndVMax(ptr->entity_vector()->vmin(), &vmin_, &vmax_);
  UpdateVMinAndVMax(ptr->entity_vector()->vmax(), &vmin_, &vmax_);
  blending_count_ += (ptr->blending() != NULL) ? 1 : 0;
}

MeshPartPtr Mesh::RemoveMeshPartAt(int32 index) {
  DCHECK(index < static_cast<int32>(part_.size()));
  MeshPartPtr ptr = part_[index];
  ptr->SetEffectAdapterContext(NULL);
  part_.erase(part_.begin() + index);
  UpdateMinAndMax();
  blending_count_ -= (ptr->blending() != NULL) ? 1 : 0;
  return ptr;
}

void Mesh::ClearMeshPart() {
  part_.clear();
  blending_count_ = 0;
}

void Mesh::Render(Renderer* renderer) {
  DCHECK(context_ || vector_.size() == 0u);
  for (auto iter = part_.begin(); iter != part_.end(); ++iter) {
    MeshPart* part = iter->get();
    ApplyParams(part->effect());
    part->Render(renderer);
  }
}

void Mesh::UpdateMinAndMax() {
  for (auto iter = part_.begin(); iter != part_.end(); ++iter) {
    MeshPart* part = iter->get();
    UpdateVMinAndVMax(part->entity_vector()->vmin(), &vmin_, &vmax_);
    UpdateVMinAndVMax(part->entity_vector()->vmax(), &vmin_, &vmax_);
  }
}

//
void UpdateVMinAndVMax(const Vector3 pos, Vector3* vmin, Vector3* vmax) {
  if (pos.x < vmin->x) vmin->x = pos.x;
  if (pos.y < vmin->y) vmin->y = pos.y;
  if (pos.z < vmin->z) vmin->z = pos.z;
  if (pos.x > vmax->x) vmax->x = pos.x;
  if (pos.y > vmax->y) vmax->y = pos.y;
  if (pos.z > vmax->z) vmax->z = pos.z;
}
}  // namespace azer
