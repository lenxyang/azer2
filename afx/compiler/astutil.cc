#include "azer/afx/compiler/astnode.h"

#include "azer/afx/compiler/astutil.h"
#include "azer/afx/compiler/context.h"

namespace azer {
namespace afx {

ValuePtr GetNodeValue(ASTNode* node) {
  if (node->IsConstNode()) {
    return ValuePtr(node->ToConstNode()->value());
  } else if (node->IsBinaryOpNode()) {
    return ValuePtr(node->ToBinaryOpNode()->value());
  } else if (node->IsUnaryOpNode()) {
    return ValuePtr(node->ToUnaryOpNode()->value());
  } else if (node->IsRefSymbolNode()) {
    RefSymbolNode* ref = node->ToRefSymbolNode();
    return ValuePtr(new Value(ref->GetDeclNode()->GetType()));
  } else if (node->IsFuncCallTypeInitNode()) {
    FuncCallTypeInitNode* typeinit = node->ToFuncCallTypeInitNode();
    DCHECK(typeinit->GetType().get() != NULL);
    return ValuePtr(new Value(typeinit->GetType()));
  } else if (node->IsFuncCallNode()) {
    FuncCallNode* call = node->ToFuncCallNode();
    DCHECK(call->GetFuncDefNode() && call->GetFuncDefNode()->IsFuncDefNode());
    DCHECK(call->GetFuncDefNode()->GetProtoNode());
    FuncProtoNode* proto = call->GetFuncDefNode()->GetProtoNode();
    return ValuePtr(new Value(proto->rettype()->GetType()));
  } else if (node->IsFieldNode()) {
    return ValuePtr(new Value(node->ToFieldNode()->GetType()));
  } else if (node->IsParamNode()) {
    return ValuePtr(new Value(node->ToParamNode()->GetType()));
  } else {
    NOTREACHED();
    return ValuePtr(new Value);
  }
}

TypedNode* GetTypedNode(ASTNode* node) {
  if (node->IsConstNode()) {
    TypedNode* typed = node->GetContext()->Create(ASTNode::kTypedNode,
                                                  node->loc())->ToTypedNode();
    typed->SetType(node->ToConstNode()->GetResultType());
    return typed;
  } else if (node->IsFuncCallNode()) {
    FuncCallNode* func = node->ToFuncCallNode();
    FuncDefNode* funcdef = func->GetFuncDefNode();
    DCHECK(funcdef != NULL);
    FuncProtoNode* proto = funcdef->GetProtoNode();
    DCHECK(proto != NULL);
    return proto->rettype();
  } else if (node->IsRefSymbolNode()) {
    SymbolNode* symbol = node->ToRefSymbolNode()->GetDeclNode();
    DCHECK(symbol != NULL);
    TypedNode* typed = symbol->GetTypedNode();
    DCHECK(typed != NULL);
    // perhaps struct field or swizzle expression
    if (typed->GetStructDecl() != NULL || typed->GetType()->IsVector()) {
      return typed;
    } else {
      return NULL;
    }
  } else if (node->IsBinaryOpNode()) {
    BinaryOpNode* binary = node->ToBinaryOpNode();
    if (binary->GetOperator() == kOpMember
        || binary->GetOperator() == kOpIndex) {
      TypedNode* typed = node->GetContext()->Create(ASTNode::kTypedNode,
                                                    node->loc())->ToTypedNode();
      typed->SetType(binary->GetResultType());
      return typed;
    } else {
      TypedNode* typed = node->GetContext()->Create(ASTNode::kTypedNode,
                                                    node->loc())->ToTypedNode();
      typed->SetType(binary->GetResultType());
      return typed;
    }
  } else {
    NOTREACHED();
    return NULL;
  }
}

TypePtr GetNodeType(ASTNode* node) {
  if (node->type() == ASTNode::kConstNode) {
    TypePtr typeptr(new Type(node->ToConstNode()->GetResultType()->type()));
    typeptr->SetStorageQualifier(kConst);
    return typeptr;
  } else if (node->IsRefSymbolNode()) {
    RefSymbolNode* ref = node->ToRefSymbolNode();
    return ref->GetDeclNode()->GetType();
  } else if (node->IsFuncCallTypeInitNode()) {
    FuncCallTypeInitNode* typeinit = node->ToFuncCallTypeInitNode();
    DCHECK(typeinit->GetType().get() != NULL);
    return typeinit->GetType();
  } else if (node->IsFuncCallNode()) {
    FuncCallNode* call = node->ToFuncCallNode();
    DCHECK(call->GetFuncDefNode() && call->GetFuncDefNode()->IsFuncDefNode());
    DCHECK(call->GetFuncDefNode()->GetProtoNode());
    FuncProtoNode* proto = call->GetFuncDefNode()->GetProtoNode();
    return proto->rettype()->GetType();
  } else if (node->IsUnaryOpNode()) {
    DCHECK(node->ToUnaryOpNode()->GetResultType().get() != NULL);
    return node->ToUnaryOpNode()->GetResultType();
  } else if (node->IsBinaryOpNode()) {
    DCHECK(node->ToBinaryOpNode()->GetResultType().get() != NULL);
    return node->ToBinaryOpNode()->GetResultType();
  } else if (node->IsFieldNode()) {
    return node->ToFieldNode()->GetType();
  } else if (node->IsParamNode()) {
    return node->ToParamNode()->GetType();
  } else if (node->IsStructDeclNode()) {
    StructDeclNode* decl = node->ToStructDeclNode();
    TypePtr type(new Type(kStructure));
    type->SetStructName(decl->struct_name());
    return type;
  } else if (node->IsDeclarationNode()) {
    return node->ToDeclarationNode()->GetType();
  } else {
    NOTREACHED();
    return NULL;
  }
}

ASTNode* GetTexDeclNode(ASTNode* node) {
  if (node->IsRefSymbolNode()) {
    SymbolNode* symbol = node->ToRefSymbolNode()->GetDeclNode();
    return symbol;
  } else if (node->IsFieldNode()) {
    if (node->parent() && node->parent()->IsBinaryOpNode()) {
      return GetTexDeclNode(node->parent());
    } else {
      NOTREACHED();
      return NULL;
    }
  } else if (node->IsBinaryOpNode()) {
    BinaryOpNode* binary = node->ToBinaryOpNode();
    if (binary->GetOperator() == kOpMember) {
      return GetTexDeclNode(binary->GetOper1());
    } else if (binary->GetOperator() == kOpIndex) {
      return GetTexDeclNode(binary->GetOper1());
    } else {
      NOTREACHED();
      return NULL;
    }
  } else {
    NOTREACHED();
    return NULL;
  }
}

TypedNode* GetTexNode(ASTNode* node) {
  DCHECK(IsNodeTypeTexture(node));
  if (node->IsRefSymbolNode()) {
    SymbolNode* symbol = node->ToRefSymbolNode()->GetDeclNode();
    return symbol->GetTypedNode();
  } else if (node->IsSymbolNode()) {
    SymbolNode* symbol = node->ToSymbolNode();
    return symbol->GetTypedNode();
  } else if (node->IsFieldNode()) {
    FieldNode* field = node->ToFieldNode();
    return field->GetTypedNode();
  } else if (node->IsBinaryOpNode()) {
    BinaryOpNode* binary = node->ToBinaryOpNode();
    if (binary->GetOperator() == kOpMember) {
      return GetTexNode(binary->GetOper2());
    } else if (binary->GetOperator() == kOpIndex) {
      return GetTexNode(binary->GetOper1());
    } else {
      NOTREACHED();
      return NULL;
    }
  } else {
    NOTREACHED();
    return NULL;
  }
}

TypePtr GetTextureType(ASTNode* node) {
  DCHECK(IsNodeTypeTexture(node));
  if (node->IsRefSymbolNode()) {
    SymbolNode* symbol = node->ToRefSymbolNode()->GetDeclNode();
    TypePtr ptr = symbol->GetType();
    return ptr;
  } else if (node->IsSymbolNode()) {
    SymbolNode* symbol = node->ToSymbolNode();
    TypePtr ptr = symbol->GetType();
    return ptr;
  } else if (node->IsBinaryOpNode()) {
    TypePtr ptr = node->ToBinaryOpNode()->GetResultType();
    return ptr;
  } else {
    NOTREACHED();
    return NULL;
  }
}

bool IsUniformTexture(ASTNode* node) {
  if (!IsNodeTypeTexture(node)) {
    return false;
  }

  if (node->IsActParamNode()) {
    return false;
  } else if (node->IsSymbolNode()) {
    return true;
  } else if (node->IsRefSymbolNode()) {
    SymbolNode* symbol = node->ToRefSymbolNode()->GetDeclNode();
    DCHECK(symbol != NULL);
    return IsUniformTexture(symbol);
  } else if (node->IsBinaryOpNode()) {
    // cannot be uniform
    // if texture is uniform, is will be put out of the uniform structure
    BinaryOpNode* binary = node->ToBinaryOpNode();
    if (binary->GetOperator() == kOpMember) {
      TypePtr type1 = GetNodeType(binary->GetOper1());
      return IsNodeTypeTexture(binary->GetOper2())
          && (type1->storage_qualifier() & kUniform);
      return true;
    } else if (binary->GetOperator() == kOpIndex) {
      return IsUniformTexture(binary->GetOper1());
    } else {
      return false;
    }
  } else {
    NOTREACHED();
    return false;
  }
}

bool IsNodeTypeTexture(ASTNode* node) {
  if (node->IsBinaryOpNode()) {
    TypePtr ptr = node->ToBinaryOpNode()->GetResultType();
    DCHECK(ptr.get() != NULL);
    return ptr->IsTexture();
  } else if (node->IsActParamNode()) {
    SymbolNode* symbol = node->ToSymbolNode();
    DCHECK(symbol != NULL);
    TypePtr ptr = symbol->GetType();
    return ptr->IsTexture();
  } else if (node->IsSymbolNode()) {
    // to generator SamplerState, symbolNode maybe as a parameters
    SymbolNode* symbol = node->ToSymbolNode();
    DCHECK(symbol != NULL);
    TypePtr ptr = symbol->GetType();
    DCHECK(ptr.get() != NULL);
    return ptr->IsTexture();
  } else if (node->IsRefSymbolNode()) {
    SymbolNode* symbol = node->ToRefSymbolNode()->GetDeclNode();
    DCHECK(symbol != NULL);
    return IsNodeTypeTexture(symbol);
  } else if (node->IsFieldNode()) {
    TypedNode* typed = node->ToFieldNode()->GetTypedNode();
    return typed->GetType()->IsTexture();
  } else if (node->IsParamNode()) {
    TypedNode* typed = node->ToParamNode()->GetTypedNode();
    return typed->GetType()->IsTexture();
  } else if (node->IsConstNode()) {
    return false;
  } else if (node->IsFuncCallNode()) {
    return false;
  } else if (node->IsUnaryOpNode()) {
    return IsNodeTypeTexture(node->ToUnaryOpNode()->GetOper());
  } else {
    NOTREACHED();
    return false;
  }

  return false;
}

bool HasTextureField(ASTNode* node) {
  DCHECK(node->IsStructDeclNode());
  ASTNode* cur = node->first_child();
  while (cur) {
    if (cur->IsFieldNode()) {
      FieldNode* field = cur->ToFieldNode();
      if (field->GetType()->IsTexture()) {
        return true;
      } else if (field->GetType()->IsStructure()) {
        return HasTextureField(field->GetTypedNode()->GetStructDecl());
      }
    }
    cur = cur->next_sibling();
  }
  return false;
}

bool HasOnlyTextureField(ASTNode* node) {
  DCHECK(node->IsStructDeclNode());
  ASTNode* cur = node->first_child();
  while (cur) {
    if (cur->IsFieldNode()) {
      FieldNode* fieldnode = cur->ToFieldNode();
      if (!fieldnode->GetType()->IsTexture()) {
        return false;
      }
    }
    cur = cur->next_sibling();
  }
  return true;
}

bool IsTextureArray(ASTNode* node) {
  if (node->IsRefSymbolNode()) {
    SymbolNode* symbol = node->ToRefSymbolNode()->GetDeclNode();
    TypedNode* typed = symbol->GetTypedNode();
    DCHECK(typed != NULL);
    
    return typed->IsArrayTyped();
  } else if (node->IsFieldNode()) {
    FieldNode* field = node->ToFieldNode();
    TypedNode* typed = field->GetTypedNode();
    DCHECK(typed != NULL);
    
    return typed->IsArrayTyped();
  } else if (node->IsBinaryOpNode()) {
    BinaryOpNode* binary = node->ToBinaryOpNode();
    if (binary->GetOperator() == kOpMember) {
      return IsTextureArray(binary->GetOper2());
    } else if (binary->GetOperator() == kOpIndex) {
      return IsTextureArray(binary->GetOper1());
    } else {
      NOTREACHED();
      return false;
    }
  } else {
    NOTREACHED();
    return false;
  }
}
}  // namespace azer
}  // namespace afx
