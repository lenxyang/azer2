#include "azer/render/gpu_program.h"

#include "azer/render/vertex_buffer.h"

namespace azer {
GpuProgram::GpuProgram(RenderPipelineStage stage, const std::string& program)
    : stage_(stage)
    , program_(program) {
}

GpuProgram::~GpuProgram() {
}

VertexGpuProgram::VertexGpuProgram(VertexDescPtr& desc, const std::string& program)
    : GpuProgram(kVertexStage, program)
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
