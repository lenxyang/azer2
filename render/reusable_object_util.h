#pragma once

#include <string>

#include "azer/base/export.h"
#include "azer/render/reusable_object.h"

namespace azer {
AZER_EXPORT GpuProgramPtr GetVertexProgramMayCreate(const std::string& name,
                                                    const std::string& program,
                                                    VertexDescPtr& vd);

AZER_EXPORT GpuProgramPtr GetPixelProgramMayCreate(const std::string& name,
                                                   const std::string& program);
}  // namespace azer
