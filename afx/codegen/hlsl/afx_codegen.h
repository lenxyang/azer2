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
  HLSLAfxCodegen();
  virtual ~HLSLAfxCodegen();

  virtual std::string GenCode(const Technique::StageInfo& shader,
                              bool comments = false) override;
 protected:
  /**
   * 生成 uniforms 类型相关的代码
   * 目前上不支持 glslang 类型的输出
   */
  std::string GenUniform(const std::vector<ASTNode*>& uniforms);
  virtual std::string GenTextureDecl(const std::vector<ASTNode*>& textures) = 0;
  /**
   * 生成指定阶段的 入口函数的代码
   */
  virtual std::string GenEntry(ASTNode* node);

  /**
   * 生成函数体
   * node 为 FuncProto 的入口节点，与生成函数头部的相同
   */
  std::string GenFuncBody(ASTNode* node);
  std::string GenFuncProto(ASTNode* node);
  /**
   * 生成依赖项的代码，其中 uniform 类型的数据将被提取出来
   * 用作生成 cbuffer
   */
  std::string GenDeps(const Technique::StageInfo& shader);
  std::string GenDepend(ASTNode* node);
  std::string GenUniDepend(ASTNode* node);
  std::string GenUniformDeps(const Technique::StageInfo& shader);
  HLSLCodeGeneratorFactory* factory_;
  /**
   * uniform 和 其他函数可能都依赖于某个指定的结构体， type_depends_
   * 用来对他们进行去重
   */
  std::set<std::string> type_depends_;
  bool comments_;
  DISALLOW_COPY_AND_ASSIGN(HLSLAfxCodegen);
};

class HLSLVSAfxCodegen : public HLSLAfxCodegen {
 public:
  HLSLVSAfxCodegen();
 private:
  virtual std::string GenTextureDecl(const std::vector<ASTNode*>& t) override;
  DISALLOW_COPY_AND_ASSIGN(HLSLVSAfxCodegen);
};

class HLSLPSAfxCodegen : public HLSLAfxCodegen {
 public:
  HLSLPSAfxCodegen();
 private:
  virtual std::string GenTextureDecl(const std::vector<ASTNode*>& t) override;
  DISALLOW_COPY_AND_ASSIGN(HLSLPSAfxCodegen);
};

class HLSLGSAfxCodegen : public HLSLAfxCodegen {
 public:
  HLSLGSAfxCodegen();

  virtual std::string GenEntry(ASTNode* node) override;
 private:
  virtual std::string GenTextureDecl(const std::vector<ASTNode*>& t) override;
  DISALLOW_COPY_AND_ASSIGN(HLSLGSAfxCodegen);
};
}  // namespace afx
}  // namespace azer
