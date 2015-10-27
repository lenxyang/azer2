#pragma once

#include <vector>
#include "base/memory/ref_counted.h"
#include "azer/render/effect.h"
#include "azer/render/effect_params_provider.h"

namespace azer {

class EffectAdapterContext;
class EffectParamsProvider;
typedef scoped_refptr<EffectParamsProvider> EffectParamsProviderPtr;
typedef std::vector<EffectParamsProviderPtr> EffectParamsProviderVector;

class Entity;
class EntityVec;
typedef scoped_refptr<Entity> EntityPtr;
typedef scoped_refptr<EntityVec> EntityVecPtr;

// class Entity
class AZER_EXPORT Entity : public ::base::RefCounted<Entity> {
 public:
  Entity();
  Entity(VertexBufferPtr vb, IndicesBufferPtr ib);
  ~Entity();
  
  void SetVertexBuffer(VertexBufferPtr vb);
  void SetIndicesBuffer(IndicesBufferPtr ib);

  void Draw(Renderer* renderer, PrimitiveTopology primitive);
  void DrawIndex(Renderer* renderer, PrimitiveTopology primitive);

  EntityPtr DeepCopy();

  const Vector3& vmin() { return vmin_;}
  const Vector3& vmax() { return vmax_;}
  Vector3* mutable_vmin() { return &vmin_;}
  Vector3* mutable_vmax() { return &vmax_;}
 private:
  VertexBufferPtr vb_;
  IndicesBufferPtr ib_;
  Vector3 vmin_;
  Vector3 vmax_;
  DISALLOW_COPY_AND_ASSIGN(Entity);
};

// class EntityVec
class AZER_EXPORT EntityVec : public ::base::RefCounted<EntityVec> {
 public:
  EntityVec();

  void AddEntity(EntityPtr ptr);
  void RemoveEntityAt(int32 index);
  Entity* entity_at(int32 index) { return vec_[index].get();}
  int32 entity_count() { return static_cast<int32>(vec_.size());}
  EntityVecPtr DeepCopy();

  const Vector3& vmin() { return vmin_;}
  const Vector3& vmax() { return vmax_;}
 private:
  void UpdateMinAndMax();
  std::vector<EntityPtr> vec_;
  Vector3 vmin_;
  Vector3 vmax_;
  DISALLOW_COPY_AND_ASSIGN(EntityVec);
};

// class MeshPart
class AZER_EXPORT MeshPart : public EffectParamsProviderContainer {
 public:
  MeshPart(Effect* effect);
  MeshPart(Effect* effect, EntityVecPtr vec);
  MeshPart(const MeshPart& part);

  MeshPart& operator = (const MeshPart& part);

  void UpdateProviderParams(const FrameArgs& args) override;
  virtual void Render(Renderer* renderer);

  Effect* effect() { return effect_.get();}
  void SetEffect(Effect* effect) { effect_ = effect;}
  EntityVec* entity_vector() { return vecptr_.get();}

  void AddEntity(EntityPtr ptr) { vecptr_->AddEntity(ptr);}
  void RemoveEntityAt(int32 index) { vecptr_->RemoveEntityAt(index);}
  Entity* entity_at(int32 index) { return vecptr_->entity_at(index);}
  int32 entity_count() { return vecptr_->entity_count();}
 private:
  EffectPtr effect_;
  EntityVecPtr vecptr_;
};
typedef scoped_refptr<MeshPart> MeshPartPtr;

class AZER_EXPORT Mesh : public EffectParamsProviderContainer {
 public:
  Mesh();
  explicit Mesh(EffectAdapterContext* context);
  ~Mesh();

  void AddMeshPart(MeshPart* entity);
  MeshPartPtr RemoveMeshPartAt(int32 index);
  void ClearMeshPart();
  int32 part_count() const { return static_cast<int32>(part_.size());}
  MeshPart* part_at(int32 index) { return part_[index].get();}

  void UpdateProviderParams(const FrameArgs& args) override;
  void Render(Renderer* renderer);
  
  const Vector3& vmin() { return vmin_;}
  const Vector3& vmax() { return vmax_;}
 private:
  void UpdateMinAndMax();
  Vector3 vmin_;
  Vector3 vmax_;
  std::vector<scoped_refptr<MeshPart> > part_;
  DISALLOW_COPY_AND_ASSIGN(Mesh);
};

typedef scoped_refptr<Mesh> MeshPtr;

void UpdateVMinAndVMax(const Vector3 pos, Vector3* vmin, Vector3* vmax);
}  // namespace azer
