#pragma once

#include <vector>
#include "base/memory/ref_counted.h"
#include "azer/render/effect.h"
#include "azer/render/effect_params_provider.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/vertex_buffer.h"

namespace azer {

class AZER_EXPORT Mesh : public ::base::RefCounted<Mesh> {
 public:
  Mesh();
  explicit Mesh(EffectAdapterContext* context);
  ~Mesh();

  struct AZER_EXPORT Entity {
    VertexBufferPtr vb;
    IndicesBufferPtr ib;
  };

  Entity* entity_at(int32 index);
  const Entity* entity_at(int32 index) const;
  int32 entity_count() const { return static_cast<int32>(entity_.size());}
  Entity RemoveEntityAt(int32 index);

  void AddEntity(Entity entity);
  void AddProvider(EffectParamsProviderPtr provider);
  void RemoveProvider(EffectParamsProviderPtr provider);
  void ResetProvider();

  void Update(const FrameArgs& args);
  void Draw(Renderer* renderer, PrimitiveTopology primitive);
  void DrawIndex(Renderer* renderer, PrimitiveTopology primitive);
 private:
  void ApplyEffectParams(Entity* entity, Renderer* renderer);

  std::vector<Entity> entity_;
  std::vector<EffectParamsProviderPtr> provider_;
  EffectAdapterContext* context_;
  DISALLOW_COPY_AND_ASSIGN(Mesh);
};

typedef scoped_refptr<Mesh> MeshPtr;
}  // namespace azer
