#pragma once

#include <memory>
#include <string>
#include <vector>

#include "azer/base/export.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/shader.h"
#include "azer/render/technique.h"
#include "azer/effect/shader_closure.h"

namespace azer {
class Effect;
class GpuConstantsTable;
class Renderer;
class RenderSystem;
typedef scoped_refptr<GpuConstantsTable> GpuConstantsTablePtr;
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
  
  // 刷新所有的 GpuConstantTable
  void SetShaderClosure(ShaderClosure* closure);
  ShaderClosure* GetShaderClosure(int stage);
  void FlushGpuVariables(int flush_mode, Renderer* renderer);

  void Apply(Renderer* renderer);
  void OnRenderBegin(Renderer* renderer);
  void OnRenderEnd(Renderer* renderer);
  void OnRenderNewObject(Renderer* renderer);
 protected:
  void BindConstantsTable(Renderer* renderer);
  void SaveShaderResource(int stage, int index, ShaderResView* tex);
  void SetGpuConstantsTable(int stage, int index, GpuConstantsTable* table);
  virtual ShaderClosurePtr InitShaderClosure(RenderPipelineStage, Shader*);
  virtual void ApplyGpuConstantTable(Renderer* renderer);

  std::vector<ShaderClosurePtr> shaders_;
  TechniquePtr technique_;
  VertexDescPtr vertex_desc_;
  DISALLOW_COPY_AND_ASSIGN(Effect);
};

class ScopedEffect {
 public:
  ScopedEffect(Effect* effect, Renderer* renderer) 
      : effect_(effect),
        renderer_(renderer) {
    effect_->OnRenderBegin(renderer);
  }

  ~ScopedEffect() {
    effect_->OnRenderEnd(renderer_);
  }
 private:
  Effect* effect_;
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(ScopedEffect);
};
}  // namespace azer
