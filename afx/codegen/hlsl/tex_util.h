#pragma once

#include <string>
#include <vector>

#include "azer/afx/compiler/type.h"

namespace azer {
namespace afx {

class ASTNode;
class TypedNode;

/**
 * 从 sampler 函数的角度计算 texture 的名称
 */
std::string GenReferredTextureVarName(ASTNode* node);

/**
 * 判断 texture 是否属于 Material
 */
bool IsMaterialTex(ASTNode* node);

/**
 * 为 node 产生一个完整的 texture 名称(包括数组后缀) 
 */
std::string HLSLRefferedTextureFullName(ASTNode* node);

/**
 * 此函数与 HLSLRefferedTextureFullName 不同的是
 * 数组部分的是声明时的维度
 */
std::string HLSLDeclaredTextureFullName(ASTNode* node);

/**
 * 获取 sampler 变量的名称
 * D3D11 在执行 sampler 时，需要指定一个 sampler, afx 为每一个 uniform 变量生成一个
 * 默认 sampler, 以下两个函数将根据 texture 节点的名称生成对应 sampler 对象的名称
 * 一种 DeclFullName，在声明时使用，除了名称之外，它还带有 array specifier
 * 另外一个函数访问时使用
 */
std::string HLSLUniformTextureSamplerDeclFullName(ASTNode* node);
std::string HLSLUniformTextureSamplerFullName(ASTNode* node);


/**
 * 设置 uniform texture 的函数名
 */
std::string HLSLTextureUniformFuncName(ASTNode* node);

/**
 * texture 后面的 array specified
 */
std::string HLSLTextureArraySpecifier(ASTNode* node);

std::string HLSLTextureTypeName(ASTNode* node);
}  // namespace afx
}  // namespace azer
