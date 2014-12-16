#include "azer/afx/linker/technique_validator.h"

#include "base/logging.h"
#include "azer/afx/compiler/astnode.h"
#include "azer/afx/linker/technique_linker.h"
#include "azer/afx/linker/attribute_name.h"
#include "azer/render/render_system_enum.h"

namespace azer {
namespace afx {

namespace {
class EmitVariableFinder : public TreeNode<ASTNode>::Traverser {
 public:
  EmitVariableFinder()  {}
  virtual bool OnVisitBegin(ASTNode* node) override {
    if (node->IsSymbolNode()) {
      SymbolNode* symbol = node->ToSymbolNode();
      CHECK(symbol && symbol->GetDeclarationNode());
      DeclarationNode* decl = symbol->GetDeclarationNode();
      if (decl->attributes() 
          && decl->attributes()->HasAttr(AttrNames::kGSEmitVariable)) {
        emit_.push_back(symbol);
      }
    }
    return true;
  }

  virtual void OnVisitEnd(ASTNode* node) override {}

  std::vector<SymbolNode*>* GetEmitVariable() { return &emit_;}
 private:
  std::vector<SymbolNode*> emit_;
  DISALLOW_COPY_AND_ASSIGN(EmitVariableFinder);
};
}  // namespace

bool TechniqueValidator::Valid(Technique* tech) {
  if (tech->shader[kVertexStage].entry == NULL) {
    std::stringstream ss;
    ss << "technique \"" << tech->name << "\"'s vs_main \""
       << tech->entry[kVertexStage] << "\" is not defined";
    linker_->ReportError(ss.str());
    return false;
  }

  if (tech->shader[kPixelStage].entry == NULL) {
    std::stringstream ss;
    ss << "technique \"" << tech->name << "\"'s vs_main \""
       << tech->entry[kPixelStage] << "\" is not defined";
    linker_->ReportError(ss.str());
    return false;
  }


  if (tech->shader[kGeometryStage].entry != NULL) {
    if (!ValidGeometryShader(&tech->shader[kGeometryStage])) {
      return false;
    }
  } else if (!tech->entry[kGeometryStage].empty()) {
    std::stringstream ss;
    ss << "technique \"" << tech->name << "\"'s gs_main \""
       << tech->entry[kGeometryStage] << "\" is not defined";
    linker_->ReportError(ss.str());
    return false;
  }
  return true;
}

bool TechniqueValidator::ValidGeometryShader(Technique::StageInfo* shader) {
  ASTNode* entry = shader->entry;
  const AttributesNode* attr = entry->attributes();
  if (!attr) {
    linker_->ReportError("geometry shader's entry has no attributes");
    return false;
  }
  
  std::string attrs[] = {
    AttrNames::kGSMaxVertexCount,
    AttrNames::kGSVertexType,
    AttrNames::kGSPrimitiveType,
    AttrNames::kGSEmitVariable,
  };
  for (uint32 i = 0; i < arraysize(attrs); ++i) {
    if (!attr->HasAttr(attrs[i])) {
      std::stringstream ss;
      ss << "geometry shader: attribute \"" << attrs[i] << "\" no specified.";
      linker_->ReportError(ss.str());
      return false;
    }
  }

  // check attributes valud
  EmitVariableFinder finder;
  shader->entry->traverse(&finder);
  if (finder.GetEmitVariable()->size() == 0u) {
    std::stringstream ss;
    ss << "geometry shader: no emit variable specified.";
    linker_->ReportError(ss.str());
    return false;
  } else if (finder.GetEmitVariable()->size() > 1u) {
    std::stringstream ss;
    ss << "geometry shader: multiply emit variable specified.";
    linker_->ReportError(ss.str());
    return false;
  }

  return true;
}

SymbolNode* GetGSEmitVariable(Technique::StageInfo* shader) {
  EmitVariableFinder finder;
  shader->entry->traverse(&finder);
  CHECK_EQ(finder.GetEmitVariable()->size(), 1u);
  return finder.GetEmitVariable()->at(0);
}
}  // namespace afx
}  // namespace azer
