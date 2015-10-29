#include "azer/render/mesh.h"

#include "base/logging.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"

namespace azer {

// class Entity
Entity::Entity() : topology_(kTriangleList) {
}

Entity::Entity(VertexBufferPtr vb, IndicesBufferPtr ib)
    : vb_(vb),
      ib_(ib),
      topology_(kTriangleList) {
}

Entity::~Entity() {
}

void Entity::Render(Renderer* renderer) {
  if (ib_.get()) {
    DrawIndex(renderer);
  } else {
    Draw(renderer);
  }
}

void Entity::Draw(Renderer* renderer) {
  renderer->UseVertexBuffer(vb_.get());
  renderer->UseIndicesBuffer(NULL);
  renderer->SetPrimitiveTopology(topology());
  renderer->Draw(0, vb_->vertex_num());
}

void Entity::DrawIndex(Renderer* renderer) {
  renderer->UseVertexBuffer(vb_.get());
  renderer->UseIndicesBuffer(ib_.get());
  renderer->SetPrimitiveTopology(topology());
  renderer->DrawIndex(ib_->indices_num(), 0, 0);
}

void Entity::SetVertexBuffer(VertexBufferPtr vb) {
  vb_ = vb;
}

void Entity::SetIndicesBuffer(IndicesBufferPtr ib) {
  ib_ = ib;
}

EntityPtr Entity::DeepCopy() {
  NOTIMPLEMENTED();
  return EntityPtr();
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

void MeshPart::UpdateProviderParams(const FrameArgs& args) {
  EffectParamsProviderContainer::UpdateProviderParams(args);
}

void MeshPart::Render(Renderer* renderer) {
  ApplyParams(effect_.get());
  effect_->Use(renderer);
  for (int32 i = 0; i < vecptr_->entity_count(); ++i) {
    Entity* entity = vecptr_->entity_at(i);
    entity->Render(renderer);
  }
}

// class Mesh
Mesh::Mesh() {
}

Mesh::Mesh(EffectAdapterContext* context)
    : EffectParamsProviderContainer(context) {
}

Mesh::~Mesh() {
}

void Mesh::AddMeshPart(MeshPart* ptr) {
  ptr->SetEffectAdapterContext(context_);
  part_.push_back(MeshPartPtr(ptr));
  UpdateVMinAndVMax(ptr->entity_vector()->vmin(), &vmin_, &vmax_);
  UpdateVMinAndVMax(ptr->entity_vector()->vmax(), &vmin_, &vmax_);
}

MeshPartPtr Mesh::RemoveMeshPartAt(int32 index) {
  DCHECK(index < static_cast<int32>(part_.size()));
  MeshPartPtr ptr = part_[index];
  ptr->SetEffectAdapterContext(NULL);
  part_.erase(part_.begin() + index);
  UpdateMinAndMax();
  return ptr;
}

void Mesh::ClearMeshPart() {
  part_.clear();
}

void Mesh::UpdateProviderParams(const FrameArgs& args) {
  EffectParamsProviderContainer::UpdateProviderParams(args);
  for (auto iter = part_.begin(); iter != part_.end(); ++iter) {
    (*iter)->UpdateProviderParams(args);
  }
}

void Mesh::Render(Renderer* renderer) {
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
