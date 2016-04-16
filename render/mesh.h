#pragma once

#include <vector>
#include "base/memory/ref_counted.h"
#include "azer/math/vector3.h"
#include "azer/effect/effect.h"
#include "azer/effect/effect_params_provider.h"
#include "azer/render/entity.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
class Blending;
class EffectAdapterContext;
class EffectParamsProvider;
typedef scoped_refptr<EffectParamsProvider> EffectParamsProviderPtr;
typedef std::vector<EffectParamsProviderPtr> EffectParamsProviderVector;
typedef scoped_refptr<Blending> BlendingPtr;

class MeshPart;
class Mesh;
typedef scoped_refptr<MeshPart> MeshPartPtr;
typedef scoped_refptr<Mesh> MeshPtr;

// class MeshPart
class AZER_EXPORT MeshPart : public EffectParamsProviderContainer {
 public:
  MeshPart(Effect* effect);
  MeshPart(Effect* effect, EntityVecPtr vec);
  MeshPart(const MeshPart& part);

  MeshPart& operator = (const MeshPart& part);

  void SetBlending(Blending* blending) { blending_ = blending;}
  Blending* blending() { return blending_.get();}
  virtual void Render(Renderer* renderer);
  void Draw(Renderer* renderer);

  Effect* effect() { return effect_.get();}
  void SetEffect(Effect* effect) { effect_ = effect;}
  EntityVec* entity_vector() { return vecptr_.get();}

  void AddEntity(EntityPtr ptr);
  void RemoveEntityAt(int32 index);
  Entity* entity_at(int32 index);
  const Entity* entity_at(int32 index) const;
  int32 entity_count() const;
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

  MeshPart* AddMeshPart(MeshPart* entity);
  MeshPartPtr RemoveMeshPartAt(int32 index);
  void ClearMeshPart();
  int32 part_count() const { return static_cast<int32>(part_.size());}
  MeshPart* part_at(int32 index) { return part_[index].get();}
  bool has_blending() const { return blending_count_ > 0;}

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

}  // namespace azer
