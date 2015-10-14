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
  explicit RenderClosure(EffectAdapterContext* context);
  
  void SetVertexBuffer(VertexBufferPtr vb);
  void SetIndicesBuffer(IndicesBufferPtr ib);

  void Draw(Renderer* renderer, Effect* effect, PrimitiveTopology primitive);
  void DrawIndex(Renderer* renderer, Effect* effect, PrimitiveTopology primitive);
 private:
  VertexBufferPtr vb_;
  IndicesBufferPtr ib_;
  DISALLOW_COPY_AND_ASSIGN(RenderClosure);
};

typedef scoped_refptr<RenderClosure> RenderClosurePtr;
}  // namespace azer
