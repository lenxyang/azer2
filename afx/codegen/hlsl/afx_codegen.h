#pragma once

#include <set>
#include <string>
#include <vector>

#include "azer/afx/codegen/afx_codegen.h"
#include "azer/afx/codegen/hlsl/ast_codegen.h"

namespace azer {
namespace afx {

class ASTNode;
class CodeGeneratorFactory;

class HLSLAfxCodegen : public AfxCodegen {
 public:
  HLSLAfxCodegen() : stage_(kStageNotSpec) {}

  std::string GenCode(RenderPipelineStage stage,
                      const TechniqueParser::StageInfo& shader,
                      bool comments = false) override;
 private:
  /**
   * 生成 uniforms 类型相关的代码
   * 目前上不支持 glslang 类型的输出
   */
  std::string GenUniform(const std::vector<ASTNode*>& uniforms, bool comments);
  std::string GenTextures(const std::vector<ASTNode*>& textures, bool comments);
  /**
   * 生成指定阶段的 入口函数的代码
   */
  std::string GenEntry(ASTNode* node, bool comments);
  /**
   * 生成依赖项的代码，其中 uniform 类型的数据将被提取出来
   * 用作生成 cbuffer
   */
  std::string GenDeps(const TechniqueParser::StageInfo& shader, bool comments);
  std::string GenDepend(ASTNode* node, bool comments);
  std::string GenUniDepend(ASTNode* node, bool comments);
  std::string GenUniformDeps(const TechniqueParser::StageInfo& shader,
                             bool comments);

  std::string GenGeometryShaderCode(const TechniqueParser::StageInfo& shader,
                                    bool comments);
  std::string GenVertexAndPixelShaderCode(RenderPipelineStage stage,
                                          const TechniqueParser::StageInfo& shader,
                                          bool comments);
  
  HLSLCodeGeneratorFactory factory_;
  RenderPipelineStage stage_;
  /**
   * uniform 和 其他函数可能都依赖于某个指定的结构体， type_depends_
   * 用来对他们进行去重
   */
  std::set<std::string> type_depends_;
  DISALLOW_COPY_AND_ASSIGN(HLSLAfxCodegen);
};
}  // namespace afx
}  // namespace azer