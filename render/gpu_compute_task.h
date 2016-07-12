#pragma once

#include <vector>


#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/math/vector3.h"
#include "azer/effect/stage_res_container.h"
#include "azer/render/shader.h"
#include "azer/render/resource_view.h"

namespace azer {
class GpuConstantsTable;
class Renderer;
class Shader;
struct ShaderInfo;
class Texture;
class TextureView;

class AZER_EXPORT GpuComputeTask : public ::base::RefCounted<GpuComputeTask> {
 public:
  GpuComputeTask(const ShaderInfo& info, int rescour, int uacount);
  virtual ~GpuComputeTask();

  void SetResource(int index, ResourceView* tex);
  void SetUAResource(int index, ResourceView* tex);
  void Reset(Renderer* renderer);
  void Bind(Renderer* renderer);
  Shader* gpu_program() { return gpu_program_.get();}
  GpuConstantsTable* constants_table() { return constants_table_.get();}
  static const int kMaxResourceCount = 32;
protected:
  scoped_refptr<Shader> gpu_program_;
  scoped_refptr<GpuConstantsTable> constants_table_;
  std::vector<ResourceViewPtr> shader_resource_;
  std::vector<ResourceViewPtr> shader_uaresource_;
  ShaderInfo shader_info_;
  DISALLOW_COPY_AND_ASSIGN(GpuComputeTask);
};

typedef scoped_refptr<GpuComputeTask> GpuComputeTaskPtr;
}  // namespace azer
