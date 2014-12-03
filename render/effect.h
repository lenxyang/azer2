#pragma once

#include <vector>
#include <memory>


#include "azer/base/export.h"
#include "azer/base/resource.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/renderer.h"
#include "azer/render/technique.h"

namespace azer {

class Renderer;
class RenderSystem;

/**
 * class Effect
 * Effect 一组经过链接的 shader 程序和这些程序需要的常量列表
 * Effect 作为一个虚基类基本上不做任何工作，此类的子类大部分是通过
 * afxcompile 程序生成的
 * 子类会生成如何初始化 shader 及 如何与常量表交互的代码
 */
class AZER_EXPORT Effect : public Resource {
 public:
  explicit Effect(RenderSystem* rs);
  virtual ~Effect() {}

  virtual void Use(Renderer* renderer);

  // 刷新所有的 GpuConstantTable
  void flush(Renderer* renderer);
 protected:
  virtual void UseTexture(Renderer* renderer) = 0;
  void UseConstantsTable(Renderer* renderer);
  void UseTechnique(Renderer* renderer);
  TechniquePtr technique_;
  std::vector<std::unique_ptr<GpuConstantsTable> > gpu_table_;
  RenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(Effect);
};

typedef std::shared_ptr<Effect> EffectPtr;

typedef int32 int2[2];
typedef int32 int3[3];
typedef int32 int4[4];

typedef uint32 uint;
typedef uint32 uint2[2];
typedef uint32 uint3[3];
typedef uint32 uint4[4];

typedef bool bool2[2];
typedef bool bool3[3];
typedef bool bool4[4];

inline void Effect::UseTechnique(Renderer* renderer) {
  DCHECK(technique_.get() != NULL);
  technique_->Use(renderer);
}

inline void Effect::UseConstantsTable(Renderer* renderer) {
  DCHECK(render_system_ != NULL);
  for (int i = (int)kVertexStage; i <= (int)kPixelStage; ++i) {
    GpuConstantsTable* table = gpu_table_[i].get();
    RenderPipelineStage stage = (RenderPipelineStage)i;
    if (table != NULL) {
      table->flush(renderer);
      renderer->UseConstantsTable(stage, table);
    }
  }
}
}  // namespace
