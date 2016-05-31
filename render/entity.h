#pragma once

#include <vector>
#include "base/memory/ref_counted.h"
#include "azer/math/vector3.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
class EntityData;
class Entity;
class EntityVec;
typedef scoped_refptr<EntityData> EntityDataPtr;
typedef scoped_refptr<Entity> EntityPtr;
typedef scoped_refptr<EntityVec> EntityVecPtr;

struct AZER_EXPORT Subset {
  int32_t vertex_base;
  int32_t vertex_count;
  int32_t index_base;
  int32_t index_count;
  PrimitiveTopology primitive;
  Vector3 vmin;
  Vector3 vmax;

  Subset();
  Subset(int32_t vbase, int32_t vcount, int32_t ibase, int32_t icount);
  Subset(int32_t vbase, int32_t vcount, int32_t ibase, int32_t icount, PrimitiveTopology type);
};
typedef std::vector<Subset> Subsets;

class AZER_EXPORT EntityData : public ::base::RefCounted<EntityData> {
 public:
  explicit EntityData(VertexData* vdata);
  EntityData(VertexData* vdata, IndicesData* idata);
  EntityData(VertexDesc* desc, int32_t count);
  ~EntityData();
  
  void AddSubset(const Subset& sub) { subset_.push_back(sub);}
  VertexData* vdata() const { return vdata_.get();}
  IndicesData* idata() const { return idata_.get();}
  const Subsets& subset() const { return subset_;}
 private:
  VertexDataPtr vdata_;
  IndicesDataPtr idata_;
  std::vector<Subset> subset_;
  DISALLOW_COPY_AND_ASSIGN(EntityData);
};

// class Entity
class AZER_EXPORT Entity : public ::base::RefCounted<Entity> {
 public:
  explicit Entity(VertexDesc* desc);
  explicit Entity(VertexBufferGroup* vb);
  explicit Entity(EntityData* data);
  Entity(VertexBuffer* vb);
  Entity(VertexBuffer* vb, IndicesBuffer* ib);
  Entity(VertexBufferGroup* vb, IndicesBuffer* ib);
  ~Entity();
  
  void SetVertexBuffer(VertexBuffer* vb, int32_t index);
  void SetIndicesBuffer(IndicesBuffer* ib);
  VertexBuffer* vertex_buffer_at(int32_t index);
  VertexBufferGroup* vertex_buffer_group() { return vbg_.get();}
  IndicesBuffer* indices_buffer() { return ib_.get();}
  const VertexDesc* vertex_desc() const;
  void AddSubset(const Subset& subset);
  void RemoveSubset(int32_t index);
  int32_t subset_count() const;
  const Subset& subset_at(int32_t index) const;

  void Draw(Renderer* renderer) const;
  void DrawSub(int32_t index, Renderer* renderer) const;

  EntityPtr DeepCopy();
  const Vector3& vmin() { return vmin_;}
  const Vector3& vmax() { return vmax_;}
  void set_vmin(const Vector3& v) { vmin_ = v;}
  void set_vmax(const Vector3& v) { vmax_ = v;}
  void UpdateBounds();
 private:
  void DrawSubset(Renderer* renderer, const Subset& subset) const;
  void DrawIndexSubset(Renderer* renderer, const Subset& subset) const;
  VertexBufferGroupPtr vbg_;
  IndicesBufferPtr ib_;
  Vector3 vmin_;
  Vector3 vmax_;
  std::vector<Subset> subset_;
  DISALLOW_COPY_AND_ASSIGN(Entity);
};

// class EntityVec
class AZER_EXPORT EntityVec : public ::base::RefCounted<EntityVec> {
 public:
  EntityVec();

  Entity* AddEntity(Entity* ptr);
  void RemoveEntityAt(int32_t index);
  Entity* entity_at(int32_t index) { return vec_[index].get();}
  int32_t entity_count() { return static_cast<int32>(vec_.size());}
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
}  // namespace azer
