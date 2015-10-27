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

class AZER_EXPORT RenderClosure {
 public:
  RenderClosure();
  explicit RenderClosure(EffectAdapterContext* context);
  
  void SetVertexBuffer(VertexBufferPtr vb);
  void SetIndicesBuffer(IndicesBufferPtr ib);

  void Draw(Renderer* renderer, PrimitiveTopology primitive);
  void DrawIndex(Renderer* renderer, PrimitiveTopology primitive);

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

class RenderClosureVec : public ::base::RefCounted<RenderClosureVec> {
 public:
  RenderClosureVec();

  void AddRenderClosure(RenderClosurePtr ptr);
  void RemoveRenderClosureAt(int32 index);

  const Vector3& vmin() { return vmin_;}
  const Vector3& vmax() { return vmax_;}
 private:
  std::vector<RenderClosurePtr> vec_;
  Vector3 vmin_;
  Vector3 vmax_;
  DISALLOW_COPY_AND_ASSIGN(RenderClosureVec);
};
typedef scoped_refptr<RenderClosureVec> RenderClosureVecPtr;
}  // namespace azer
