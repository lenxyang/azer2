#pragma once

#include <string>
#include <memory>
#include <algorithm>

#include <d3d11.h>

#include "base/basictypes.h"
#include "azer/render_system/d3d11/dx3d_util.h"
#include "azer/render/common.h"
#include "azer/base/string.h"
#include "base/logging.h"


namespace azer {

ID3DBlob* CompileShaderForStage(RenderPipelineStage stage, 
                                const std::string& shader, 
                                const std::string& path, 
                                std::string* error_msg);
const char* DefaultShaderEntryForStage(RenderPipelineStage stage);

const char* HLSLTypeName(DataFormat format);
}  // namespace azer
