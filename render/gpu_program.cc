#include "azer/render/gpu_program.h"

#include "base/logging.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
ShaderInfo::ShaderInfo() : stage(kStageNotSpec) {}
  
GpuProgram::GpuProgram(const ShaderInfo& info)
    : stage_((RenderPipelineStage)info.stage)
    , info_(info) {
}

GpuProgram::~GpuProgram() {
}

VertexGpuProgram::VertexGpuProgram(VertexDescPtr& desc, const ShaderInfo& info)
    : GpuProgram(info)
    , desc_ptr_(desc) {
  DCHECK_EQ(kVertexStage, info.stage);
}

CharType* ShaderSuffix(ShaderType type) {
  switch (type) {
    case kBinaryShader: return AZER_LITERAL("bin");
    case kStringShader: return AZER_LITERAL("str");
    default: NOTREACHED(); return AZER_LITERAL("");
  }
}
}  // namespace azer
