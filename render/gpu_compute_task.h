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

  void SetInputResource(int index, ResourceView* tex);
  void SetInputUAResource(int index, ResourceView* tex);
  void SetOutputTexture(int index, TextureView* tex);
  void Reset();
  Shader* gpu_program() { return gpu_program_.get();}
  GpuConstantsTable* constants_table() { return constants_table_.get();}

  static const int kMaxInput = 32;
  static const int kMaxOutput = 32;
 protected:
  scoped_refptr<Shader> gpu_program_;
  scoped_refptr<GpuConstantsTable> constants_table_;
  StageResContainer input_;
  StageResContainer uainput_;
  StageResContainer output_;
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
