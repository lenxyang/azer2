#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/base/export.h"
#include "azer/render/vertex_data.h"
#include "azer/render/gpu_program.h"

namespace azer {

class Renderer;

class AZER_EXPORT Technique : public ::base::RefCounted<Technique> {
 public:
  virtual ~Technique() {}
  void AddGpuProgram(GpuProgramPtr& gpu) {
    DCHECK_LT(gpu->stage(), kRenderPipelineStageNum);
    pline_[gpu->stage()] = gpu;
  }

  void AddGpuProgram(VertexGpuProgramPtr& gpu) {
    DCHECK_EQ(gpu->stage(), kVertexStage);
    pline_[gpu->stage()] = gpu;
  }

  virtual void Use(Renderer*) = 0;
  VertexDescPtr GetVertexDesc();
 protected:
  Technique() {
    pline_.resize(kRenderPipelineStageNum);
  }

  std::vector<GpuProgramPtr> pline_;
  DISALLOW_COPY_AND_ASSIGN(Technique);
};

typedef scoped_refptr<Technique> TechniquePtr;

inline VertexDescPtr Technique::GetVertexDesc() {
  GpuProgramPtr& ptr = pline_[kVertexStage];
  if (ptr.get()) {
    return ptr->GetInputDesc();
  } else {
    return NULL;
  }
}
}  // namespace azer
