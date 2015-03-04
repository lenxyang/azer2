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

typedef scoped_refptr<Effect> EffectPtr;
}  // namespace azer
