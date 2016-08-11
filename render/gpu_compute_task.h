#pragma once

#include <vector>

#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/math/vector3.h"
#include "azer/render/gpu_resource_view.h"
#include "azer/render/shader.h"
#include "azer/render/resource_view.h"

namespace azer {
class ShaderParamTable;
class Renderer;
class Shader;
class Texture;
class TextureView;
struct ShaderInfo;
typedef scoped_refptr<ShaderParamTable> ShaderParamTablePtr;

class AZER_EXPORT GpuComputeTask : public ::base::RefCounted<GpuComputeTask> {
 public:
  GpuComputeTask(const ShaderInfo& info, int table_count, int rescour, int uacount);
  virtual ~GpuComputeTask();

  void SetResource(int index, ShaderResView* tex);
  void SetUAResource(int index, UnorderAccessView* tex);
  void Reset(Renderer* renderer);
  void Bind(Renderer* renderer);
  Shader* gpu_program() { return gpu_program_.get();}
  ShaderParamTable* constants_table(int i);
  void SetShaderParamTable(int index, ShaderParamTable* table);
  static const int kMaxResourceCount = 32;
protected:
  scoped_refptr<Shader> gpu_program_;
  std::vector<ShaderParamTablePtr> constants_table_;
  std::vector<ShaderResViewPtr> shader_resource_;
  std::vector<UnorderAccessViewPtr> shader_uaresource_;
  ShaderInfo shader_info_;
  DISALLOW_COPY_AND_ASSIGN(GpuComputeTask);
};

typedef scoped_refptr<GpuComputeTask> GpuComputeTaskPtr;
}  // namespace azer
