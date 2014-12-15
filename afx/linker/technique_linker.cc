#include "azer/afx/linker/technique_linker.h"

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
  
  std::string vs_entry = node->GetAttrValue(AttrNames::kVertexShaderEntry);
  std::string hs_entry = node->GetAttrValue(AttrNames::kHullShaderEntry);
  std::string ds_entry = node->GetAttrValue(AttrNames::kDomainShaderEntry);
  std::string gs_entry = node->GetAttrValue(AttrNames::kGeometryShaderEntry);
  std::string ps_entry = node->GetAttrValue(AttrNames::kPixelShaderEntry);
  ASTNode* vs_node = vs_entry.empty() ? NULL : context->GetFunctionByName(vs_entry);
  ASTNode* gs_node = gs_entry.empty() ? NULL : context->GetFunctionByName(gs_entry);
  ASTNode* hs_node = hs_entry.empty() ? NULL : context->GetFunctionByName(gs_entry);
  ASTNode* ds_node = ds_entry.empty() ? NULL : context->GetFunctionByName(gs_entry);
  ASTNode* ps_node = ps_entry.empty() ? NULL : context->GetFunctionByName(ps_entry);
  
  technique.attributes = node;
  technique.entry[kVertexStage] = vs_entry;
  technique.entry[kPixelStage] = ps_entry;
  technique.entry[kGeometryStage] = gs_entry;
  technique.entry[kHullStage] = hs_entry;
  technique.entry[kDomainStage] = ds_entry;

  technique.shader[kVertexStage].entry = vs_node;
  technique.shader[kPixelStage].entry = ps_node;
  technique.shader[kGeometryStage].entry = gs_node;
  technique.shader[kHullStage].entry = hs_node;
  technique.shader[kDomainStage].entry = ds_node;
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
