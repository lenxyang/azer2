#pragma once

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "azer/render/common.h"
#include "azer/render/gpu_resource_view.h"
#include "azer/render/shader_param_table.h"

namespace azer {
class Shader;
class Renderer;
typedef scoped_refptr<Shader> ShaderPtr;

class AZER_EXPORT ShaderClosure : public ::base::RefCounted<ShaderClosure> {
 public:
  explicit ShaderClosure(RenderPipelineStage stage);

  RenderPipelineStage stage() const { return stage_;}
  void SetShader(Shader* shader, int table_count, int res_count, int uares_count);
  void SetResource(int index, ShaderResViewPtr tex);
  void SetUAResource(int index, UnorderAccessViewPtr tex);
  void SetShaderParamTable(int index, ShaderParamTablePtr table);

  int table_count() const { return static_cast<int>(table_.size());}
  ShaderParamTable* table_at(int index);
  void UpdateRes(Renderer* renderer);
  void UpdateUARes(Renderer* renderer);
  void UpdateShaderParam(int index, Renderer* renderer);
  void UpdateAllShaderParam(Renderer* renderer);
  void Bind(Renderer* renderer);
  void Reset(Renderer* renderer);
 protected:
  std::vector<ShaderParamTablePtr> table_;
  std::vector<ShaderResViewPtr> shader_res_;
  std::vector<UnorderAccessViewPtr> shader_uares_;
  ShaderPtr shader_;
  const RenderPipelineStage stage_;
  DISALLOW_COPY_AND_ASSIGN(ShaderClosure);
};

typedef scoped_refptr<ShaderClosure> ShaderClosurePtr;
}  // namespace azer
