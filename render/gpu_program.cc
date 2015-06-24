#include "azer/render/gpu_program.h"

#include "azer/render/vertex_buffer.h"

namespace azer {
GpuProgram::ShaderInfo::ShaderInfo()
    : stage(kStageNotSpec) {
}
  
GpuProgram::GpuProgram(RenderPipelineStage stage, const ShaderInfo& info)
    : stage_(stage)
    , info_(info) {
}

GpuProgram::~GpuProgram() {
}

VertexGpuProgram::VertexGpuProgram(VertexDescPtr& desc, const ShaderInfo& info)
    : GpuProgram(kVertexStage, info)
    , desc_ptr_(desc) {
}

CharType* ShaderSuffix(ShaderType type) {
  switch (type) {
    case kBinaryShader: return AZER_LITERAL("bin");
    case kStringShader: return AZER_LITERAL("str");
    default: NOTREACHED(); return AZER_LITERAL("");
  }
}
}  // namespace azer
