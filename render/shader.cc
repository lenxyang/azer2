#include "azer/render/shader.h"

#include "base/logging.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "azer/base/file_system.h"
#include "azer/base/res_path.h"
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

bool LoadShaderOnFS(int stage, const ResPath& path, 
                        ShaderInfo* shader, FileSystem* fs) {
  FileContents contents;
  if (!LoadFileContents(path, &contents, fs)) {
    return false;
  }

  shader->stage = stage;
  shader->path = base::UTF16ToUTF8(path.fullpath()).c_str();
  shader->code = std::string((const char*)&contents.front(), contents.size());
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

bool LoadShaderOnFS(int stage, const ResPath& path, TechSource* shaders, 
                    FileSystem* fs) {
  ShaderInfo shader;
  if (!LoadShaderOnFS(stage, path, &shader, fs)) {
    return false;
  }

  shaders->SetStage(stage, shader);
  return true;
}
}  // namespace azer
