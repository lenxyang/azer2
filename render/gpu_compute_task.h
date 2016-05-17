#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/shader.h"

namespace azer {
class Shader;
class GpuConstantsTable;
class Texture;
class TextureView;
struct ShaderInfo;

class AZER_EXPORT GpuComputeTask : public ::base::RefCounted<GpuComputeTask> {
 public:
  explicit GpuComputeTask(const ShaderInfo& info);
  virtual ~GpuComputeTask();

  int input_count() const { return input_count_;}
  int output_count() const { return output_count_;}
  void SetInputCount(int32 count) { input_count_ = count;}
  void SetOutputCount(int32 count) { output_count_ = count;}
  void SetInputTexture(int32 index, TextureView* tex);
  void SetOutputTexture(int32 index, TextureView* tex);
  void Reset();

  TextureView* GetInputAt(int32 index);
  TextureView* GetOutputAt(int32 index);
  Shader* gpu_program() { return gpu_program_;}
  GpuConstantsTable* constants_table() { return constants_table_;}

  static const int32 kMaxInput = 1024;
  static const int32 kMaxOutput = 1024;
 protected:
  scoped_refptr<Shader> gpu_program_;
  scoped_refptr<GpuConstantsTable> constants_table_;
  TextureView* input_[kMaxInput];
  TextureView* output_[kMaxOutput];
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
