#include "azer/render/technique.h"

#include "azer/render/shader.h"
#include "azer/render/render_system.h"
#include "azer/render/vertex_buffer.h"


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
  return GetStage(index);
}

const ShaderInfo& TechSource::GetStage(const int32 index) const {
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
    return ptr->vertex_desc();
  } else {
    return NULL;
  }
}

void Technique::AddShader(ShaderPtr& gpu) {
  DCHECK_LT(gpu->stage(), kRenderPipelineStageNum);
  pline_[gpu->stage()] = gpu;
}

TechniquePtr CreateTechnique(const TechSource& source, RenderSystem* rs) {
  TechniquePtr tech = rs->CreateTechnique();  
  const ShaderInfo& vsinfo = source.GetStage(kVertexStage);
  ShaderPtr vs_shader(rs->CreateVertexShader(source.vertex_desc(), vsinfo));
  tech->AddShader(vs_shader);
  for (uint32 i = (uint32)(kVertexStage + 1); i < source.size(); ++i) {
    const ShaderInfo& info = source.GetStage(i);
    if (!info.code.empty()) {
      ShaderPtr ptr(rs->CreateShader(info));
      CHECK(ptr.get() != NULL);
      tech->AddShader(ptr);
    }
  }
  return tech;
}
}  // namespace azer
