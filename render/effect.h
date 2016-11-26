#pragma once

#include <memory>
#include <string>
#include <vector>

#include "azer/base/export.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/shader.h"
#include "azer/render/technique.h"
#include "azer/render/shader_closure.h"

namespace azer {
class Effect;
class ShaderParamTable;
class Renderer;
class RenderSystem;
typedef scoped_refptr<ShaderParamTable> ShaderParamTablePtr;
typedef scoped_refptr<Effect> EffectPtr;

/**
 * class Effect
 * Effect 一组经过链接的 shader 程序和这些程序需要的常量列表
 * Effect 作为一个虚基类基本上不做任何工作，此类的子类大部分是通过
 * afxcompile 程序生成的
 * 子类会生成如何初始化 shader 及 如何与常量表交互的代码
 */
class AZER_EXPORT Effect : public ::base::RefCounted<Effect> {
 public:
  enum {
    kUpdatePerOject = 0x00000001,
    kUpdatePerFrame = 0x00000002,
    kUpdatePerScene = 0x00000004,
    kUpdateAll      = (kUpdatePerScene | kUpdatePerOject | kUpdatePerFrame),
  };
  
  Effect();
  virtual ~Effect();

  virtual const char* GetEffectName() const = 0;
  virtual bool Init(const TechSource& programs);

  Technique* technique();
  void SetTechnique(Technique* technique);
  VertexDesc* vertex_desc();
  const VertexDesc* vertex_desc() const;
  void SetVertexDesc(VertexDesc* desc);
  
  // 刷新所有的 ShaderParamTable
  void SetShaderClosure(ShaderClosure* closure);
  ShaderClosure* GetShaderClosure(int stage);
  
  /// 更新函数
  void UpdateRes(int stage, Renderer* renderer);
  void UpdateAllRes(Renderer* renderer);
  void UpdateGpuParams(int stage, int index, Renderer* renderer);
  void UpdateAllGpuParams(Renderer* renderer);

  void Apply(Renderer* renderer);
 protected:
  void SaveShaderResource(int stage, int index, ShaderResView* tex);
  void SetShaderParamTable(int stage, int index, ShaderParamTable* table);

  // 为每一个 shader 创建 ShaderClosure
  ShaderClosurePtr InitShaderClosure(RenderPipelineStage, Shader*);
  virtual void OnBeforeShaderClosureInit();
  virtual void OnAfterShaderClosureInit();
  virtual ShaderClosurePtr InitVertexStage(Shader* shader);
  virtual ShaderClosurePtr InitHullStage(Shader* shader);
  virtual ShaderClosurePtr InitDomainStage(Shader* shader);
  virtual ShaderClosurePtr InitGeometryStage(Shader* shader);
  virtual ShaderClosurePtr InitPixelStage(Shader* shader);
  virtual ShaderClosurePtr InitComputeStage(Shader* shader);

  // 设置 shader 的参数的值
  virtual void ApplyShaderParamTable(Renderer* renderer);

  std::vector<ShaderClosurePtr> shaders_;
  TechniquePtr technique_;
  VertexDescPtr vertex_desc_;
  DISALLOW_COPY_AND_ASSIGN(Effect);
};
}  // namespace azer
