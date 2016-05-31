#include "azer/afx/codegen/util.h"

#include "azer/afx/compiler/astnode.h"
#include "azer/afx/compiler/astutil.h"
#include <sstream>

namespace azer {
namespace afx {

class TextureNameGenerator : public TreeNode<ASTNode>::Traverser {
 public:
  TextureNameGenerator() {}

  void GenCode(ASTNode* node);
  const std::string& GetName() const { return code_gen_;}
  const std::vector<std::string>& GetFields() const { return fields_;}
 protected:
  virtual bool OnVisitBegin(ASTNode* node) override;
  virtual void OnVisitEnd(ASTNode* node) override;
 private:
  std::string code_gen_;
  std::vector<std::string> fields_;
  DISALLOW_COPY_AND_ASSIGN(TextureNameGenerator);
};

void TextureNameGenerator::GenCode(ASTNode* node) {
  code_gen_.clear();
  node->traverse(this);
}

bool TextureNameGenerator::OnVisitBegin(ASTNode* node) {
  if (node->IsBinaryOpNode()) {
    TextureNameGenerator gen1;
    TextureNameGenerator gen2;
    BinaryOpNode* binary = node->ToBinaryOpNode();
    if (binary->GetOperator() == kOpMember) {
      gen1.GenCode(binary->GetOper1());
      gen2.GenCode(binary->GetOper2());
      code_gen_.append(gen1.GetName());
      code_gen_.append("_");
      code_gen_.append(gen2.GetName());

      fields_.push_back(gen1.GetName());
      fields_.push_back(gen2.GetName());
    } else {
      NOTREACHED();
    }
    return false;
  } else if (node->IsRefSymbolNode()) {
    RefSymbolNode* symbol = node->ToRefSymbolNode();
    code_gen_.append(symbol->symbolname());
    fields_.push_back(symbol->symbolname());
    return false;
  } else if (node->IsFieldNode()) {
    FieldNode* field = node->ToFieldNode();
    code_gen_.append("_");
    code_gen_.append(field->fieldname());
    fields_.push_back(field->fieldname());
    return false;
  } else {
    NOTREACHED();
    return false;
  }
}

void TextureNameGenerator::OnVisitEnd(ASTNode* node)  {
}

std::string GenReferredTextureVarName(ASTNode* node) {
  DCHECK(IsNodeTypeTexture(node));
  if (node->IsRefSymbolNode()) {
    return node->ToRefSymbolNode()->symbolname();
  } else if (node->IsSymbolNode()) {
    return node->ToSymbolNode()->symbolname();
  } else if (node->IsParamNode()) {
    return node->ToParamNode()->paramname();
  } else if (node->IsBinaryOpNode()) {
    TextureNameGenerator gen;
    BinaryOpNode* binary = node->ToBinaryOpNode();
    DCHECK(binary);
    gen.GenCode(binary->GetOper1());
    return gen.GetName();
  } else {
    NOTREACHED();
    return "";
  }
}

void GetTexRefferPath(ASTNode* node, std::vector<std::string>* path) {
  DCHECK(IsNodeTypeTexture(node));
  if (node->IsRefSymbolNode()) {
    path->push_back(node->ToRefSymbolNode()->symbolname());
    return;
  } else if (node->IsSymbolNode()) {
    path->push_back(node->ToSymbolNode()->symbolname());
    return;
  } else if (node->IsBinaryOpNode()) {
    TextureNameGenerator gen;
    BinaryOpNode* binary = node->ToBinaryOpNode();
    DCHECK(binary);
    gen.GenCode(binary->GetOper1());
    *path = gen.GetFields();
  } else {
    NOTREACHED();
  }
}
}  // namespace afx
}  // namespace azer
