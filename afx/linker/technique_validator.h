#pragma once


#include "azer/afx/linker/technique.h"

namespace azer {
namespace afx {

class TechniqueLinker;

/**
 * class TechniqueValidator
 * 用于检测 Technique 是否有不合法的地方
 * 具体检测规则包括
 * 1. geometry shader
 *   gs 是否指定了最大的顶点数量
 *   gs 的 entry 函数的格式是否符合标准
 **/
class TechniqueValidator {
 public:
  TechniqueValidator(TechniqueLinker* linker);
  bool Valid(Technique* technique);
 private:
  bool ValidGeometryShader(Technique::StageInfo* shader);

  /**
   * 检查各个阶段的数据类型是否一致
   * 例如, 仅有 VertexShader 和 PixelShader 的 afx 程序
   * VertexShader 的返回类型应该与 PixelShader 的输入类型一致
   */
  bool ValidExchangeType();
  TechniqueLinker* linker_;
  DISALLOW_COPY_AND_ASSIGN(TechniqueValidator);
};

inline TechniqueValidator::TechniqueValidator(TechniqueLinker* linker)
    : linker_(linker) {
}

SymbolNode* GetGSEmitVariable(ASTNode* node);
SymbolNode* GetGSEmitVariable(const Technique& tech);

/**
 * 从 gs 主函数中的任意一个节点寻找 emit variable
 */
SymbolNode* GetGSEmitVariableUpstream(ASTNode* node);
}  // namespace afx
}  // namespace azer
