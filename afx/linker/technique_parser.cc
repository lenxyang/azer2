#include "azer/afx/linker/technique_parser.h"

#include <set>
#include <sstream>

#include "azer/afx/linker/depend_calactor.h"
#include "azer/afx/linker/attribute_name.h"
#include "azer/afx/linker/technique_validator.h"
#include "azer/afx/compiler/astnode.h"
#include "azer/afx/compiler/context.h"
#include "azer/afx/compiler/util.h"
#include "azer/render/render_system_enum.h"

namespace azer {
namespace afx {

TechniqueLinker::TechniqueLinker() {
}

TechniqueLinker::~TechniqueLinker() {
}

bool TechniqueLinker::GenTechnique(AttributesNode* node, ParseContext* context) {
  DCHECK(context->root());
  DCHECK(context->success());
  Technique technique;
  const std::string& name = node->GetAttrValue(AttrNames::kName);
  std::string ps_main = node->GetAttrValue(AttrNames::kPixelShaderEntry);
  std::string vs_main = node->GetAttrValue(AttrNames::kVertexShaderEntry);
  if (ps_main.empty()) {
    std::stringstream ss;
    ss << "technique \"" << name << "\"'s psmain is empty";
    ReportError(ss.str());
    return false;
  }

  if (vs_main.empty()) {
    std::stringstream ss;
    ss << "technique \"" << name << "\"'s vsmain is empty";
    ReportError(ss.str());
    return false;
  }

  ASTNode* ps_node = context->GetFunctionByName(ps_main);
  ASTNode* vs_node = context->GetFunctionByName(vs_main);
  if (!ps_node) {
    std::stringstream ss;
    ss << "technique \"" << name << "\"'s ps_main \""
       << ps_main << "\" is not defined";
    ReportError(ss.str());
    return false;
  }

  if (!vs_node) {
    std::stringstream ss;
    ss << "technique \"" << name << "\"'s vs_main \""
       << vs_main << "\" is not defined";
    ReportError(ss.str());
    return false;
  }

  technique.shader.resize(kRenderPipelineStageNum);
  technique.attributes = node;
  technique.shader[kVertexStage].entry = vs_node;
  technique.shader[kPixelStage].entry = ps_node;
  technique.name = name;
  CalcFuncDeps(&technique);
  TechniqueValidator validator(this);
  if (validator.Valid(technique)) {
    techniques_.insert(std::make_pair(name, technique));
    return true;
  } else {
    return false;
  }
}

void TechniqueLinker::ReportError(const std::string& errtext) {
  error_text_ = errtext;
}

bool TechniqueLinker::Parse(ParseContext* context) {
  // find the attributes
  for (auto iter = context->GetTechniques().begin();
       iter != context->GetTechniques().end(); ++iter) {
    AttributesNode* attr = iter->second->ToAttributesNode();
    if (!GenTechnique(attr, context)) {
      return false;
    }
  }

  return true;
}

bool TechniqueLinker::GetTechnique(const std::string& name,
                                    Technique** technique) {
  auto iter = techniques_.find(name);
  if (iter == techniques_.end()) return NULL;
  *technique = &iter->second;
  return true;
}

void TechniqueLinker::CalcFuncDeps(Technique* technique) {
  for (auto iter = technique->shader.begin(); iter != technique->shader.end();
       ++iter) {
    if (iter->entry) {
      DependCalactor calc;
      calc.Calc(iter->entry);
      calc.Swap(&(*iter));
    }
  }
}
}  // namespace afx
}  // namespace azer
