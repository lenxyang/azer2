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

  int input_count() const { return input_count_;}
  int output_count() const { return output_count_;}
  void SetInputCount(int32 count) { input_count_ = count;}
  void SetOutputCount(int32 count) { output_count_ = count;}
  void SetInputTexture(int32 index, Texture* tex);
  void SetOutputTexture(int32 index, Texture* tex);
  void Reset();

  Texture* GetInputAt(int32 index);
  Texture* GetOutputAt(int32 index);
  GpuProgram* gpu_program() { return gpu_program_;}
  GpuConstantsTable* constants_table() { return constants_table_;}

  static const int32 kMaxInput = 1024;
  static const int32 kMaxOutput = 1024;
 protected:
  scoped_refptr<GpuProgram> gpu_program_;
  scoped_refptr<GpuConstantsTable> constants_table_;
  Texture* input_[kMaxInput];
  Texture* output_[kMaxOutput];
  int32 input_count_;
  int32 output_count_;
  ShaderInfo shader_info_;
  DISALLOW_COPY_AND_ASSIGN(GpuComputeTask);
};

typedef scoped_refptr<GpuComputeTask> GpuComputeTaskPtr;

struct GpuTaskParams {
  uint32 thread_group_x;
  uint32 thread_group_y;
  uint32 thread_group_z;
};

class AZER_EXPORT GpuComputeTaskDispatcher :
      public ::base::RefCounted<GpuComputeTaskDispatcher> {
 public:
  GpuComputeTaskDispatcher();
  virtual void Reset() = 0;
  virtual void Dispatch(GpuComputeTask* task, const GpuTaskParams params) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(GpuComputeTaskDispatcher);
};
typedef scoped_refptr<GpuComputeTaskDispatcher> GpuComputeTaskDispatcherPtr;
}  // namespace azer
