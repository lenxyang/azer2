#include "azer/render/shader.h"

#include "base/logging.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "azer/render/technique.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
ShaderInfo::ShaderInfo() : stage(kStageNotSpec) {}
  
// class Shader
Shader::Shader(const ShaderInfo& info) : info_(info) {}
Shader::Shader(VertexDesc* desc, const ShaderInfo& info)
    : info_(info),
      desc_(desc) {
}

Shader::~Shader() {}
RenderPipelineStage Shader::stage() const { return (RenderPipelineStage)info_.stage;}

CharType* ShaderSuffix(ShaderType type) {
  switch (type) {
    case kBinaryShader: return AZER_LITERAL("bin");
    case kStringShader: return AZER_LITERAL("str");
    default: NOTREACHED(); return AZER_LITERAL("");
  }
}

bool LoadShader(int stage, const std::string& path, ShaderInfo* shader) {
  ::base::FilePath fpath(::base::UTF8ToUTF16(path));
  DCHECK(shader != NULL);
  std::string code;
  if (!::base::ReadFileToString(fpath, &code)) {
    return false;
  }

  shader->code = code;
  shader->path = path;
  shader->stage = stage;
  return true;
}

bool LoadShader(int stage, const std::string& path, TechSource* shaders) {
  ShaderInfo shader;
  if (!LoadShader(stage, path, &shader)) {
    return false;
  }

  shaders->SetStage(stage, shader);
  return true;
}

}  // namespace azer
