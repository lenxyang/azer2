#pragma once

#include <string>
#include <vector>

#include "azer/afx/compiler/type.h"

namespace azer {
namespace afx {

/**
 * 获得指定节点的类型（不进行任何计算）
 */
ValuePtr GetNodeValue(ASTNode* node);
TypePtr GetNodeType(ASTNode* node);
TypedNode* GetTypedNode(ASTNode* node);


bool IsTextureArray(ASTNode* node);

/**
 * 获得 texture 的声明 node
 * 如果是结构体的成员，则返回结构体的 symbol 节点
 * 如果仅仅是一个变量，则返回变量的声明节点
 */
ASTNode* GetTexDeclNode(ASTNode* node);

/**
 * 获取最终的 Texture 节点的类型节点
 * 例如 mtrl.tex.diffuse[0]， 最终节点是 diffuse 这个 FieldNode
 */
TypedNode* GetTexNode(ASTNode* node);

/**
 * 获取的 Texture 节点的 Type
 * 注： Texture 有可能是 Texture1D, Texture2D 等，还有可能是数组
 */
TypePtr GetTextureType(ASTNode* node);

bool IsUniformTexture(ASTNode* node);

/**
 * 判断函数的参数是否是 Texture(最终结果是否是 texture 类型)
 */
bool IsNodeTypeTexture(ASTNode* node);

bool HasTextureField(ASTNode* field);
bool HasOnlyTextureField(ASTNode* node);

ScopedNode* GetScopedNode(ASTNode* node);

StructDeclNode* GetStructDecl(const std::string& name, ParseContext* context);

/**
 * 如果 typed 是一个 struct, ApplyTypedNodeDecl 帮助寻找 struct 的声明节点
 * 并将信息保存在 TypedNode 当中
 */
bool ApplyTypedNodeDecl(ASTNode* node, TypedNode* typed);
bool ApplyRefSymbolNodeSymbol(RefSymbolNode* node);
}  // namespace afx
}  // namespace azer
