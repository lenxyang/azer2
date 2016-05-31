#include "azer/render/mesh.h"

#include "base/logging.h"
#include "azer/effect/effect_params_adapter.h"
#include "azer/render/blending.h"
#include "azer/render/bounding_volumn.h"
#include "azer/render/entity.h"
#include "azer/render/renderer.h"
#include "azer/render/render_system.h"
#include "azer/render/vertex_buffer.h"

namespace azer {

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

void MeshPart::AddEntity(EntityPtr ptr) { vecptr_->AddEntity(ptr);}
void MeshPart::RemoveEntityAt(int32_t index) { vecptr_->RemoveEntityAt(index);}
Entity* MeshPart::entity_at(int32_t index) { return vecptr_->entity_at(index);}
const Entity* MeshPart::entity_at(int32_t index) const {
    return vecptr_->entity_at(index);
}
int32_t MeshPart::entity_count() const  { return vecptr_->entity_count();}

void MeshPart::Draw(Renderer* renderer) {
  for (int32_t i = 0; i < vecptr_->entity_count(); ++i) {
    Entity* entity = vecptr_->entity_at(i);
    entity->Draw(renderer);
  }
}

void MeshPart::RenderPart(Renderer* renderer) {
  DCHECK(effect_.get()) << "MeshPart Entity cannot be NULL";
  ApplyParams(effect_.get());
  renderer->BindEffect(effect_.get());
  Draw(renderer);
}

void MeshPart::Render(Renderer* renderer) {
  DCHECK(context_ || providers_.size() == 0u);
  if (!blending_.get()) {
    RenderPart(renderer);
  } else {
    scoped_ptr<ScopedResetBlending> autoblending_;
    autoblending_.reset(new ScopedResetBlending(renderer));
    renderer->SetBlending(blending_.get(), 0, 0xffffffff);
    RenderPart(renderer);
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

MeshPart* Mesh::AddMeshPart(MeshPart* ptr) {
  ptr->SetEffectAdapterContext(context_);
  part_.push_back(MeshPartPtr(ptr));
  UpdateVMinAndVMax(ptr->entity_vector()->vmin(), &vmin_, &vmax_);
  UpdateVMinAndVMax(ptr->entity_vector()->vmax(), &vmin_, &vmax_);
  blending_count_ += (ptr->blending() != NULL) ? 1 : 0;
  return ptr;
}

MeshPartPtr Mesh::RemoveMeshPartAt(int32_t index) {
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
  DCHECK(context_ || providers_.size() == 0u);
  for (auto iter = part_.begin(); iter != part_.end(); ++iter) {
    MeshPart* part = iter->get();
    ApplyParams(part->effect());
    part->Render(renderer);
  }
}

void Mesh::Draw(Renderer* renderer) {
  for (auto iter = part_.begin(); iter != part_.end(); ++iter) {
    MeshPart* part = iter->get();
    part->Draw(renderer);
  }
}

void Mesh::UpdateMinAndMax() {
  for (auto iter = part_.begin(); iter != part_.end(); ++iter) {
    MeshPart* part = iter->get();
    UpdateVMinAndVMax(part->entity_vector()->vmin(), &vmin_, &vmax_);
    UpdateVMinAndVMax(part->entity_vector()->vmax(), &vmin_, &vmax_);
  }
}

}  // namespace azer
