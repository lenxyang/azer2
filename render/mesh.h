#pragma once

#include <vector>
#include "base/memory/ref_counted.h"
#include "azer/render/effect.h"
#include "azer/render/effect_params_provider.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/vertex_buffer.h"

namespace azer {

struct AZER_EXPORT EntityData {
  VertexDataPtr vdata;
  IndicesDataPtr idata;
  EffectPtr effect;
  std::vector<EffectParamsProviderPtr> provider;
  std::vector<EffectParamsAdapter*> adapter;
};

struct AZER_EXPORT EntityBuffer {
  VertexBufferPtr vb;
  IndicesBufferPtr ib;
  EffectPtr effect;
  std::vector<EffectParamsProviderPtr> provider;
  std::vector<EffectParamsAdapter*> adapter;
};

class AZER_EXPORT Mesh : public ::base::RefCounted<Mesh> {
 public:
  struct AZER_EXPORT Entity {
    VertexBufferPtr vb;
    IndicesBufferPtr ib;
    int32 group_index;
  };

  struct AZER_EXPORT Group {
    EffectPtr effect;
    std::vector<int32> provider;
    std::vector<int32> adapter;
  };

  Mesh();
  ~Mesh();

  void AddEntityData(EntityData* data);
  void AddEntityBuffer(EntityBuffer* data);

  void Update(const FrameArgs& args);
  void Draw(Renderer* renderer, PrimitiveTopology primitive);
  void DrawIndex(Renderer* renderer, PrimitiveTopology primitive);
 private:
  int32 LookupAdapter(EffectParamsAdapter* adapter);
  int32 LookupProvider(EffectParamsProvider* provider);
  int32 LookupAdapterOrInsert(EffectParamsAdapter* adapter);
  int32 LookupProviderOrInsert(EffectParamsProvider* provider);

  void ApplyEffectParams(int32 group_index, Renderer* renderer);
  std::vector<Entity> entity_;
  std::vector<Group> group_;
  std::vector<EffectParamsProviderPtr> provider_;
  std::vector<EffectParamsAdapter*> adapter_;
  DISALLOW_COPY_AND_ASSIGN(Mesh);
};

typedef scoped_refptr<Mesh> MeshPtr;
}  // namespace azer
