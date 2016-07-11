#pragma once

#include <vector>


#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/shader.h"
#include "azer/render/resource_view.h"

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
  void SetInputCount(int32_t count) { input_count_ = count;}
  void SetOutputCount(int32_t count) { output_count_ = count;}
  void SetInputTexture(int32_t index, TextureView* tex);
  void SetInputUAView(int32_t index, TextureView* tex);
  void SetOutputTexture(int32_t index, TextureView* tex);
  void Reset();

  TextureView* GetInputAt(int32_t index);
  TextureView* GetOutputAt(int32_t index);
  Shader* gpu_program() { return gpu_program_.get();}
  GpuConstantsTable* constants_table() { return constants_table_.get();}

  static const int32_t kMaxInput = 1024;
  static const int32_t kMaxOutput = 1024;
 protected:
  scoped_refptr<Shader> gpu_program_;
  scoped_refptr<GpuConstantsTable> constants_table_;
  TextureView* input_[kMaxInput];
  TextureView* output_[kMaxOutput];
  int32_t input_count_;
  int32_t output_count_;
  ShaderInfo shader_info_;
  DISALLOW_COPY_AND_ASSIGN(GpuComputeTask);
};

typedef scoped_refptr<GpuComputeTask> GpuComputeTaskPtr;

struct GpuTaskParams {
  uint32_t thread_group_x;
  uint32_t thread_group_y;
  uint32_t thread_group_z;
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
