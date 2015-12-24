#pragma once

#include <vector>
#include "base/memory/ref_counted.h"
#include "azer/render/effect.h"
#include "azer/render/effect_params_provider.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
class Blending;
class EffectAdapterContext;
class EffectParamsProvider;
typedef scoped_refptr<EffectParamsProvider> EffectParamsProviderPtr;
typedef std::vector<EffectParamsProviderPtr> EffectParamsProviderVector;
typedef scoped_refptr<Blending> BlendingPtr;

class Entity;
class EntityVec;
class MeshPart;
class Mesh;
typedef scoped_refptr<Entity> EntityPtr;
typedef scoped_refptr<EntityVec> EntityVecPtr;
typedef scoped_refptr<MeshPart> MeshPartPtr;
typedef scoped_refptr<Mesh> MeshPtr;

// class Entity
class AZER_EXPORT Entity : public ::base::RefCounted<Entity> {
 public:
  explicit Entity(VertexDesc* desc);
  explicit Entity(VertexBufferGroup* vb);
  Entity(VertexDesc* desc, VertexBuffer* vb);
  Entity(VertexDesc* desc, VertexBuffer* vb, IndicesBuffer* ib);
  Entity(VertexBufferGroup* vb, IndicesBuffer* ib);
  ~Entity();
  
  void SetVertexBuffer(VertexBuffer* vb, int32 index);
  void SetIndicesBuffer(IndicesBuffer* ib);
  VertexBuffer* vertex_buffer_at(int32 index);
  VertexBufferGroup* vertex_buffer_group() { return vbg_.get();}
  IndicesBuffer* indices_buffer() { return ib_.get();}

  void Draw(Renderer* renderer);
  void DrawIndex(Renderer* renderer);
  virtual void Render(Renderer* renderer);

  EntityPtr DeepCopy();

  const Vector3& vmin() { return vmin_;}
  const Vector3& vmax() { return vmax_;}
  Vector3* mutable_vmin() { return &vmin_;}
  Vector3* mutable_vmax() { return &vmax_;}

  PrimitiveTopology topology() const { return topology_;}
  void set_topology(PrimitiveTopology top) { topology_ = top;}
 private:
  VertexBufferGroupPtr vbg_;
  IndicesBufferPtr ib_;
  PrimitiveTopology topology_;
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

  void SetBlending(Blending* blending) { blending_ = blending;}
  Blending* blending() { return blending_.get();}
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
  void RenderPart(Renderer* renderer);
  EffectPtr effect_;
  EntityVecPtr vecptr_;
  BlendingPtr blending_;
};

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
  bool has_blending() const { return blending_count_ > 0;}

  void UpdateProviderParams(const FrameArgs& args) override;
  virtual void Render(Renderer* renderer);
  
  const Vector3& vmin() { return vmin_;}
  const Vector3& vmax() { return vmax_;}
 private:
  void UpdateMinAndMax();
  Vector3 vmin_;
  Vector3 vmax_;
  int32 blending_count_;
  std::vector<MeshPartPtr> part_;
  DISALLOW_COPY_AND_ASSIGN(Mesh);
};

void AZER_EXPORT UpdateVMinAndVMax(const Vector3 pos, Vector3* vmin, Vector3* vmax);
}  // namespace azer
