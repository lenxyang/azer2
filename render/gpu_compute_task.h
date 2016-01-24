#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"

namespace azer {
class GpuProgram;
class GpuConstantsTable;
class Texture;

class AZER_EXPORT GpuComputeTask : public ::base::RefCounted<GpuComputeTask> {
 public:
  GpuComputeTask();
  virtual ~GpuComputeTask();

  virtual void SetInputTexture(int32 index, Texture* tex);
  virtual void SetOutputTexture(int32 index, Texture* tex);
  virtual void Dispatch(); 
 private:
  scoped_refptr<GpuProgram> gpu_program_;
  scoped_refptr<GpuConstantsTable> constants_table_;
  DISALLOW_COPY_AND_ASSIGN(GpuComputeTask);
};

typedef scoped_refptr<GpuComputeTask> GpuComputeTaskPtr;
}  // namespace azer
