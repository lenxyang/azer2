#pragma once

#include <vector>
#include "base/memory/ref_counted.h"
#include "azer/render/effect.h"
#include "azer/render/effect_params_provider.h"

namespace azer {

class RenderClosure;
class EffectAdapterContext;
class EffectParamsProvider;
typedef scoped_refptr<RenderClosure> RenderClosurePtr;
typedef scoped_refptr<EffectParamsProvider> EffectParamsProviderPtr;
typedef std::vector<EffectParamsProviderPtr> EffectParamsProviderVector;

class AZER_EXPORT MeshEntity : public EffectParamsProviderContainer {
 public:
  MeshEntity(Effect* effect);
  void UpdateProviderParams(const FrameArgs& args) override;
  virtual void Render(Renderer* renderer);

  MeshEntityPtr Clone();
  RenderClosureVec* render_closure() { return vecptr_.get();}
 private:
  EffectPtr effect_;
  RenderClosureVecPtr vecptr_;

  DISALLOW_COPY_AND_ASSIGN(MeshEntity);
};

class AZER_EXPORT Mesh : public EffectParamsProviderContainer {
 public:
  Mesh();
  explicit Mesh(EffectAdapterContext* context);
  ~Mesh();

  void AddMeshEntity(MeshEntity* entity);
  void RemoveMeshEntityAt(int32 index);
  void ClearMeshEntity();
  int32 entity_count() const { static_cast<int32>(entity_.size());}
  MeshEntity* entity_at(int32 index) { return entity_[index].get();}

  void UpdateProviderParams(const FrameArgs& args) override;
  void Render(Renderer* renderer);
  
  const Vector3& vmin() { return vmin_;}
  const Vector3& vmax() { return vmax_;}
 private:
  Vector3 vmin_;
  Vector3 vmax_;
  std::vector<scoped_refptr<MeshEntity> > closure_;
  DISALLOW_COPY_AND_ASSIGN(Mesh);
};

typedef scoped_refptr<Mesh> MeshPtr;
}  // namespace azer
