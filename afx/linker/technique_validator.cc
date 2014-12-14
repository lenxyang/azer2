#include "azer/afx/linker/technique_validator.h"

#include "base/logging.h"
#include "azer/afx/compiler/astnode.h"
#include "azer/afx/linker/technique_parser.h"
#include "azer/render/render_system_enum.h"

namespace azer {
namespace afx {

bool TechniqueValidator::Valid(const Technique& technique) {
  if (technique.shader[kGeometryStage].entry != NULL) {
    if (!ValidGeometryShader(technique.shader[kGeometryStage])) {
      return false;
    }
  }
  return true;
}

bool TechniqueValidator::ValidGeometryShader(const Technique::StageInfo& shader) {
  return true;
}
}  // namespace afx
}  // namespace azer
