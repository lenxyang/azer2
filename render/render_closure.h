#pragma once

#include <vector>
#include "base/memory/ref_counted.h"
#include "azer/render/effect.h"
#include "azer/render/effect_params_provider.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
class EffectAdapterContext;
class EffectAdapterCache;

class AZER_EXPORT RenderClosure : public EffectParamsProviderContainer {
 public:
  RenderClosure();
  explicit RenderClosure(EffectAdapterContext* context);
  
  void SetVertexBuffer(VertexBufferPtr vb);
  void SetIndicesBuffer(IndicesBufferPtr ib);

  virtual void Render(Renderer* renderer, Effect* effect);
  void Draw(Renderer* renderer, Effect* effect, PrimitiveTopology primitive);
  void DrawIndex(Renderer* renderer, Effect* effect, PrimitiveTopology primitive);

  const Vector3& vmin() { return vmin_;}
  const Vector3& vmax() { return vmax_;}
  Vector3* mutable_vmin() { return &vmin_;}
  Vector3* mutable_vmax() { return &vmax_;}
 private:
  VertexBufferPtr vb_;
  IndicesBufferPtr ib_;
  Vector3 vmin_;
  Vector3 vmax_;
  DISALLOW_COPY_AND_ASSIGN(RenderClosure);
};

typedef scoped_refptr<RenderClosure> RenderClosurePtr;
}  // namespace azer
