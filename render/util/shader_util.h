#pragma once

#include "azer/render/effect.h"


namespace azer {
bool LoadShaderAtStage(int stage, const std::string& path,
                       Effect::ShaderPrograms* shader);

bool LoadShader(const std::string& path_prefix, Effect::ShaderPrograms* shader);

}  // namespace azer
