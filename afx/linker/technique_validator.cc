#include "azer/afx/linker/technique_validator.h"

#include "base/logging.h"
#include "azer/afx/compiler/astnode.h"
#include "azer/afx/linker/technique_linker.h"
#include "azer/afx/linker/attribute_name.h"
#include "azer/render/render_system_enum.h"

namespace azer {
namespace afx {

bool TechniqueValidator::Valid(const Technique& technique) {
  if (technique.shader[kVertexStage].entry == NULL) {
    std::stringstream ss;
    ss << "technique \"" << technique.name << "\"'s vs_main \""
       << technique.entry[kVertexStage] << "\" is not defined";
    linker_->ReportError(ss.str());
    return false;
  }

  if (technique.shader[kPixelStage].entry == NULL) {
    std::stringstream ss;
    ss << "technique \"" << technique.name << "\"'s vs_main \""
       << technique.entry[kPixelStage] << "\" is not defined";
    linker_->ReportError(ss.str());
    return false;
  }


  if (technique.shader[kGeometryStage].entry != NULL) {
    if (!ValidGeometryShader(technique.shader[kGeometryStage])) {
      return false;
    }
  } else if (!technique.entry[kGeometryStage].empty()) {
    std::stringstream ss;
    ss << "technique \"" << technique.name << "\"'s gs_main \""
       << technique.entry[kGeometryStage] << "\" is not defined";
    linker_->ReportError(ss.str());
    return false;
  }
  return true;
}

bool TechniqueValidator::ValidGeometryShader(const Technique::StageInfo& shader) {
  const ASTNode* entry = shader.entry;
  if (!entry->attributes() || 
      !entry->attributes()->HasAttr(AttrNames::kMaxVertexCount)) {
    linker_->ReportError("geometry shader's entry not specified maxvertexcount");
    return false;
  }
  return true;
}
}  // namespace afx
}  // namespace azer
