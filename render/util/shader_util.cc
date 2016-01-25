#include "azer/render/util/shader_util.h"

#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "azer/render/render_system.h"

namespace azer {
bool LoadShaderAtStage(int stage, const std::string& path,
                       Effect::ShaderPrograms* shader) {
  DCHECK(shader->size() == 0u || shader->size() == kRenderPipelineStageNum);
  shader->resize(kRenderPipelineStageNum);
  ::base::FilePath fpath(::base::UTF8ToUTF16(path));
  DCHECK(shader != NULL);
  std::string code;
  if (!::base::ReadFileToString(fpath, &code)) {
    return false;
  }

  shader->at(stage).code = code;
  shader->at(stage).path = path;;
  shader->at(stage).stage = stage;
  return true;
}

}  // namespace azer
