#include "azer/render/gpu_program.h"

#include "base/logging.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "azer/base/file_system.h"
#include "azer/base/res_path.h"
#include "azer/render/technique.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
StageShader::StageShader() : stage(kStageNotSpec) {}
  
// class GpuProgram
GpuProgram::GpuProgram(const StageShader& info)
    : stage_((RenderPipelineStage)info.stage)
    , info_(info) {
}

GpuProgram::~GpuProgram() {
}

VertexGpuProgram::VertexGpuProgram(VertexDescPtr& desc, const StageShader& info)
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

bool LoadStageShader(int stage, const std::string& path, StageShader* shader) {
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

bool LoadStageShaderOnFS(int stage, const ResPath& path, 
                         StageShader* shader, FileSystem* fs) {
  FileContents contents;
  if (!LoadFileContents(path, &contents, fs)) {
    return false;
  }

  shader->stage = stage;
  shader->path = base::UTF16ToUTF8(path.fullpath()).c_str();
  shader->code = std::string((const char*)&contents.front(), contents.size());
  return true;
}

bool LoadStageShader(int stage, const std::string& path, TechSource* shaders) {
  StageShader shader;
  if (!LoadStageShader(stage, path, &shader)) {
    return false;
  }

  shaders->SetStageShader(stage, shader);
  return true;
}

bool LoadStageShaderOnFS(int stage, const ResPath& path, 
                         TechSource* shaders, FileSystem* fs) {
  StageShader shader;
  if (!LoadStageShaderOnFS(stage, path, &shader, fs)) {
    return false;
  }

  shaders->SetStageShader(stage, shader);
  return true;
}
}  // namespace azer
