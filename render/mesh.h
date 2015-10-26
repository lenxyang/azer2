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

class AZER_EXPORT Mesh : public EffectParamsProviderContainer {
 public:
  Mesh();
  explicit Mesh(EffectAdapterContext* context);
  ~Mesh();

  void AddRenderClosure(RenderClosurePtr ptr);

  void UpdateParams(const FrameArgs& args) override;
  void Draw(Renderer* renderer, Effect* effect, PrimitiveTopology primitive);
  void DrawIndex(Renderer* renderer, Effect* effect, PrimitiveTopology primitive);

  const Vector3& vmin() { return vmin_;}
  const Vector3& vmax() { return vmax_;}
  Vector3* mutable_vmin() { return &vmin_;}
  Vector3* mutable_vmax() { return &vmax_;}
 private:
  Vector3 vmin_;
  Vector3 vmax_;
  std::vector<RenderClosurePtr> closure_;
  DISALLOW_COPY_AND_ASSIGN(Mesh);
};

typedef scoped_refptr<Mesh> MeshPtr;
}  // namespace azer
