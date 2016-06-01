#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "azer/afx/linker/technique.h"

namespace azer {
namespace afx {
class ParseContext;

class TechniqueLinker {
 public:
  TechniqueLinker();
  ~TechniqueLinker();

  /**
   * 根据 attributes 的定义找到入口点并由入口点开始生成代码
   * 首先生成的倚赖的代码
   **/
  bool Parse(ParseContext* context);
  typedef std::map<std::string, Technique> TechniqueDict;
  bool GetTechnique(const std::string& name, Technique** technique);
  const TechniqueDict& GetTechniques() const { return techniques_;}

  const std::string& error_text() const { return error_text_;}
 private:
  void CalcFuncDeps(Technique* technique);
  void ReportError(const std::string& errtext);
  bool GenTechnique(AttributesNode* node, ParseContext* context);

  /**
   * 检查 Technique 是否合法
   * 1. 检查各个阶段主函数的参数输入如果是结构体，那么它是否是 exchange 类型的
   * Technique 的各个属性值是否合法
   */
  // TODO
  bool Valid() { return true;}
  ASTNodeVec pipeline_;
  TechniqueDict techniques_;
  std::string error_text_;

  friend class TechniqueValidator;
  DISALLOW_COPY_AND_ASSIGN(TechniqueLinker);
};
}  // namespace afx
}  // namespace azer
