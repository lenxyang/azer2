#pragma once

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "azer/render/common.h"

namespace azer {
class GpuConstantsTable;
class Shader;
class ShaderResView;
class Renderer;
class UnorderAccessResView;
typedef scoped_refptr<Shader> ShaderPtr;
typedef scoped_refptr<GpuConstantsTable> GpuConstantsTablePtr;
typedef scoped_refptr<ShaderResView> ShaderResViewPtr;
typedef scoped_refptr<UnorderAccessResView> UnorderAccessResViewPtr;

class ShaderClosure : public ::base::RefCounted<ShaderClosure> {
 public:
  explicit ShaderClosure(RenderPipelineStage stage);

  RenderPipelineStage stage() const { return stage_;}
  void SetShader(Shader* shader, int table_count, int res_count, int uares_count);
  void SetResource(int index, int count, ShaderResView* tex);
  void SetUAResource(int index, int count, UnorderAccessResView* tex);
  void SetGpuConstantsTable(int index, int count, GpuConstantsTable* table);

  GpuConstantsTable* table(int index);
  void UpdateShaderParams(Renderer* renderer);
  void Bind(Renderer* renderer);
  void Reset(Renderer* renderer);
 protected:
  std::vector<GpuConstantsTablePtr> table_;
  std::vector<ShaderResViewPtr> shader_res_;
  std::vector<UnorderAccessResViewPtr> shader_uares_;
  ShaderPtr shader_;
  const RenderPipelineStage stage_;
  DISALLOW_COPY_AND_ASSIGN(ShaderClosure);
};

typedef scoped_refptr<ShaderClosure> ShaderClosurePtr;
}  // namespace azer
