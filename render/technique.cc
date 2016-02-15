#include "azer/render/technique.h"

#include "azer/render/vertex_buffer.h"
#include "azer/render/gpu_program.h"

namespace azer {
TechSource::Options::Options() 
    : use_streamout(false) {
}

TechSource::TechSource(VertexDesc* desc) 
    : options_(Options()),
      vertex_desc_(desc) {
}

TechSource::TechSource(VertexDesc* vertex_desc, VertexDesc* streamout_desc, 
                 const Options& options) 
    : options_(options),
      vertex_desc_(vertex_desc),
      streamout_desc_(streamout_desc) {
}

const StageShader& TechSource::operator[](const int32 index) const {
  DCHECK_LT(index, kRenderPipelineStageNum);
  return shaders_[index];
}

void TechSource::SetStageShader(int32 stage, const StageShader& shader) {
  DCHECK_LT(stage, kRenderPipelineStageNum);
  shaders_[stage] = shader;
  shaders_[stage].stage = stage;
}

// class Technique
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
