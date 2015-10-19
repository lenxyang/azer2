#pragma once

#include <memory>
#include <string>
#include <vector>

#include "azer/base/export.h"
#include "azer/base/resource.h"
#include "azer/render/light.h"
#include "azer/render/material.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/gpu_program.h"

namespace azer {

class GpuConstantsTable;
class Renderer;
class RenderSystem;
class Technique;
typedef scoped_refptr<GpuConstantsTable> GpuConstantsTablePtr;
typedef scoped_refptr<Technique> TechniquePtr;

/**
 * class Effect
 * Effect 一组经过链接的 shader 程序和这些程序需要的常量列表
 * Effect 作为一个虚基类基本上不做任何工作，此类的子类大部分是通过
 * afxcompile 程序生成的
 * 子类会生成如何初始化 shader 及 如何与常量表交互的代码
 */
class AZER_EXPORT Effect : public Resource {
 public:
  typedef std::vector<GpuProgram::ShaderInfo> ShaderPrograms;
  explicit Effect(RenderSystem* rs);
  ~Effect() override;

  virtual const char* name() const = 0;
  virtual bool Init(const ShaderPrograms& programs) = 0;
  void Use(Renderer* renderer);

  // light and material didn't have a fixed structure
  // all will be dependent its implementation.
  virtual void SetLight(int light_index, Light* light) {}
  virtual void SetMatrial(Material* material) {}

  VertexDesc* GetVertexDesc() { return vertex_desc_ptr_.get();}
  
  // 刷新所有的 GpuConstantTable
  void flush(Renderer* renderer);
 protected:
  virtual void UseTexture(Renderer* renderer);
  virtual void ApplyGpuConstantTable(Renderer* renderer) = 0;
  void InitShaders(const ShaderPrograms& sources);
  void UseConstantsTable(Renderer* renderer);
  void UseTechnique(Renderer* renderer);

  TechniquePtr technique_;
  std::vector<scoped_refptr<GpuConstantsTable> > gpu_table_;
  RenderSystem* render_system_;
  VertexDescPtr vertex_desc_ptr_;
  DISALLOW_COPY_AND_ASSIGN(Effect);
};

typedef scoped_refptr<Effect> EffectPtr;
}  // namespace azer
