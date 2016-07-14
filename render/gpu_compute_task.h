#pragma once

#include <vector>

#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/math/vector3.h"
#include "azer/effect/stage_res_container.h"
#include "azer/render/gpu_resource_view.h"
#include "azer/render/shader.h"
#include "azer/render/resource_view.h"

namespace azer {
class GpuConstantsTable;
class Renderer;
class Shader;
class Texture;
class TextureView;
struct ShaderInfo;
typedef scoped_refptr<GpuConstantsTable> GpuConstantsTablePtr;

class AZER_EXPORT GpuComputeTask : public ::base::RefCounted<GpuComputeTask> {
 public:
  GpuComputeTask(const ShaderInfo& info, int rescour, int uacount,
                 int table_count = 1);
  virtual ~GpuComputeTask();

  void SetResource(int index, ShaderResView* tex);
  void SetUAResource(int index, UnorderAccessResView* tex);
  void Reset(Renderer* renderer);
  void Bind(Renderer* renderer);
  Shader* gpu_program() { return gpu_program_.get();}
  GpuConstantsTable* constants_table(int i);
  void SetGpuConstantsTable(int index, GpuConstantsTable* table);
  static const int kMaxResourceCount = 32;
protected:
  scoped_refptr<Shader> gpu_program_;
  std::vector<GpuConstantsTablePtr> constants_table_;
  std::vector<ShaderResViewPtr> shader_resource_;
  std::vector<UnorderAccessResViewPtr> shader_uaresource_;
  ShaderInfo shader_info_;
  DISALLOW_COPY_AND_ASSIGN(GpuComputeTask);
};

typedef scoped_refptr<GpuComputeTask> GpuComputeTaskPtr;
}  // namespace azer
