#include "azer/render/technique.h"

#include "azer/render/vertex_buffer.h"
#include "azer/render/shader.h"

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

const ShaderInfo& TechSource::operator[](const int32 index) const {
  DCHECK_LT(index, kRenderPipelineStageNum);
  return shaders_[index];
}

void TechSource::AddShader(const ShaderInfo& shader) {
  SetStage(shader.stage, shader);
}

void TechSource::SetStage(int32 stage, const ShaderInfo& shader) {
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

VertexDesc* Technique::GetVertexDesc() {
  ShaderPtr& ptr = pline_[kVertexStage];
  if (ptr.get()) {
    return ptr->GetInputDesc();
  } else {
    return NULL;
  }
}

void Technique::AddShader(ShaderPtr& gpu) {
  DCHECK_LT(gpu->stage(), kRenderPipelineStageNum);
  pline_[gpu->stage()] = gpu;
}

void Technique::AddShader(VertexShaderPtr& gpu) {
  DCHECK_EQ(gpu->stage(), kVertexStage);
  pline_[gpu->stage()] = gpu;
}
}  // namespace azer
