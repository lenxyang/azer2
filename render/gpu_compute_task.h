#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/gpu_program.h"

namespace azer {
class GpuProgram;
class GpuConstantsTable;
class Texture;
struct ShaderInfo;

class AZER_EXPORT GpuComputeTask : public ::base::RefCounted<GpuComputeTask> {
 public:
  explicit GpuComputeTask(const ShaderInfo& info);
  virtual ~GpuComputeTask();

  virtual void SetInputTexture(int32 index, Texture* tex) = 0;
  virtual void SetOutputTexture(int32 index, Texture* tex) = 0;
  virtual void Dispatch(uint32 thread_group_x, uint32 thread_group_y,
                        uint32 thread_group_z) = 0; 
 protected:
  scoped_refptr<GpuProgram> gpu_program_;
  scoped_refptr<GpuConstantsTable> constants_table_;
  ShaderInfo shader_info_;
  DISALLOW_COPY_AND_ASSIGN(GpuComputeTask);
};

typedef scoped_refptr<GpuComputeTask> GpuComputeTaskPtr;
}  // namespace azer
