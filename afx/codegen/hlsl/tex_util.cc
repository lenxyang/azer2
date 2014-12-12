#include "azer/afx/codegen/hlsl/tex_util.h"

#include "azer/afx/compiler/astnode.h"
#include "azer/afx/compiler/astutil.h"
#include "azer/afx/codegen/hlsl/ast_codegen.h"
#include "azer/afx/codegen/hlsl/util.h"
#include "base/logging.h"

namespace azer {
namespace afx {
namespace {

const char* AfxD3DSamplerSupfix = "__d3d_sampler";
}

std::string HLSLUniformTextureSamplerDeclFullName(ASTNode* node) {
  std::stringstream ss;
  ss << GenReferredTextureVarName(node) << AfxD3DSamplerSupfix;
  TypedNode* typed = GetTexNode(node);
  if (typed->GetType()->IsArray()) {
    ss << DumpArraySpecifier(typed->GetType());
  }
  return ss.str();
}

std::string HLSLDeclaredTextureFullName(ASTNode* node) {
  DCHECK(IsNodeTypeTexture(node));
  TypedNode* typed = GetTexNode(node);
  DCHECK(typed != NULL);
  std::stringstream ss;
  ss << GenReferredTextureVarName(node);
  if (typed->GetType()->IsArray()) {
    ss << DumpArraySpecifier(typed->GetType());
  }

  return ss.str();
}

std::string HLSLTextureTypeName(ASTNode* node) {
  DCHECK(IsNodeTypeTexture(node));
  TypePtr ptr = GetTextureType(node);
  return HLSLTypeName(ptr);
}

std::string HLSLRefferedTextureFullName(ASTNode* node) {
  std::stringstream ss;
  ss << GenReferredTextureVarName(node);
  if (node->IsBinaryOpNode()) {
    BinaryOpNode* binary = node->ToBinaryOpNode();
    if (binary->GetOperator() == kOpIndex) {
      HLSLCodeGeneratorFactory factory;
      SnippetCodeGenerator codegen(&factory);
      codegen.GenCode(binary->GetOper2());
      ss << "[" << codegen.GetCode() << "]";
    } else if (binary->GetOperator() == kOpMember) {
    } else {
      NOTREACHED();
    }
  }

  return ss.str();
}

std::string HLSLUniformTextureSamplerFullName(ASTNode* node) {
  std::stringstream ss;
  ss << GenReferredTextureVarName(node) << AfxD3DSamplerSupfix;
  if (node->IsBinaryOpNode()) {
    BinaryOpNode* binary = node->ToBinaryOpNode();
    if (binary->GetOperator() == kOpIndex) {
      HLSLCodeGeneratorFactory factory;
      SnippetCodeGenerator codegen(&factory);
      codegen.GenCode(binary->GetOper2());
      ss << "[" << codegen.GetCode() << "]";
    } else if (binary->GetOperator() == kOpMember) {
    } else {
      NOTREACHED();
    }
  }

  return ss.str();
}

std::string HLSLTextureArraySpecifier(ASTNode* node) {
  if (node->IsRefSymbolNode()) {
    SymbolNode* symbol = node->ToRefSymbolNode()->GetDeclNode();
    TypedNode* typed = symbol->GetTypedNode();
    DCHECK(typed != NULL);
    
    if (typed->IsArrayTyped()) {
      return DumpArraySpecifier(typed->GetType());
    } else {
      return "";
    }
  } else if (node->IsFieldNode()) {
    FieldNode* field = node->ToFieldNode();
    TypedNode* typed = field->GetTypedNode();
    DCHECK(typed != NULL);
    
    if (typed->IsArrayTyped()) {
      return DumpArraySpecifier(typed->GetType());
    } else {
      return "";
    }
  } else if (node->IsBinaryOpNode()) {
    BinaryOpNode* binary = node->ToBinaryOpNode();
    if (binary->GetOperator() == kOpMember) {
      return HLSLTextureArraySpecifier(binary->GetOper2());
    } else if (binary->GetOperator() == kOpIndex) {
      return HLSLTextureArraySpecifier(binary->GetOper1());
    } else {
      NOTREACHED();
      return "";
    }
  } else {
    NOTREACHED();
    return "";
  }
}

bool IsMaterialTex(ASTNode* node) {
  ASTNode* decl = GetTexDeclNode(node);
  DCHECK(decl != NULL);
  DCHECK(decl->IsSymbolNode());
  SymbolNode* symbol = decl->ToSymbolNode();
  TypedNode* typed = symbol->GetTypedNode();
  DCHECK(typed != NULL);
  if (typed->GetType()->IsStructure()) {
    StructDeclNode* decl = typed->GetStructDecl();
    return decl->GetContext()->package() == "afx"
        && decl->struct_name() == "Material";
  } else {
    return false;
  }
}

std::string HLSLTextureUniformFuncName(ASTNode* node) {
  if (node->IsRefSymbolNode()) {
    SymbolNode* symbol = node->ToRefSymbolNode()->GetDeclNode();
    DeclarationNode* decl = symbol->GetDeclarationNode();
    DCHECK(decl->GetType()->IsTexture());
    DCHECK(decl->last_child()->IsSymbolNode());
    if (decl->attributes() && decl->attributes()->HasAttr("uniform_func")) {
      return decl->attributes()->GetAttrValue("uniform_func");
    } else {
      return "";
    }
  } else if (node->IsFieldNode()) {
    FieldNode* field = node->ToFieldNode();
    DCHECK(field->GetType()->IsTexture());
    if (field->attributes() && field->attributes()->HasAttr("uniform_func")) {
      return field->attributes()->GetAttrValue("uniform_func");
    } else {
      return "";
    }
  } else if (node->IsBinaryOpNode()) {
    BinaryOpNode* binary = node->ToBinaryOpNode();
    if (binary->GetOperator() == kOpMember) {
      return HLSLTextureUniformFuncName(binary->GetOper2());
    } else if (binary->GetOperator() == kOpIndex) {
      return HLSLTextureUniformFuncName(binary->GetOper1());
    } else {
      NOTREACHED();
      return "";
    }
  } else {
    NOTREACHED();
    return "";
  }
}
}  // namespace afx
}  // namespace azer
