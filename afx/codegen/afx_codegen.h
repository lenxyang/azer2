#pragma once

#include <set>
#include <string>
#include <vector>

#include "azer/afx/linker/technique_linker.h"


namespace azer {
namespace afx {

class ASTNode;
class CodeGeneratorFactory;

class AfxCodegen {
 public:
  AfxCodegen() {}
  virtual ~AfxCodegen() {}

  virtual std::string GenCode(const Technique::StageInfo& shader,
                              bool comments = false) = 0;

  static AfxCodegen* Create(const std::string& target_lan,
                            RenderPipelineStage stage);
};

}  // namespace afx
}  // namespace azer
