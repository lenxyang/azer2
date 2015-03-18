#include "azer/render/technique.h"

#include "azer/render/vertex_buffer.h"
#include "azer/render/gpu_program.h"

namespace azer {
Technique::Technique() {
  pline_.resize(kRenderPipelineStageNum);
}

Technique::~Technique() {
}

VertexDescPtr Technique::GetVertexDesc() {
  GpuProgramPtr& ptr = pline_[kVertexStage];
  if (ptr.get()) {
    return ptr->GetInputDesc();
  } else {
    return NULL;
  }
}

void Technique::AddGpuProgram(GpuProgramPtr& gpu) {
  DCHECK_LT(gpu->stage(), kRenderPipelineStageNum);
  pline_[gpu->stage()] = gpu;
}

void Technique::AddGpuProgram(VertexGpuProgramPtr& gpu) {
  DCHECK_EQ(gpu->stage(), kVertexStage);
  pline_[gpu->stage()] = gpu;
}
}  // namespace azer
