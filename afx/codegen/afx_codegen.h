#pragma once

#include <set>
#include <string>
#include <vector>

#include "azer/afx/linker/technique_parser.h"
#include "base/basictypes.h"

namespace azer {
namespace afx {

class ASTNode;
class CodeGeneratorFactory;

class AfxCodegen {
 public:
  AfxCodegen() {}
  virtual ~AfxCodegen() {}

  virtual std::string GenCode(RenderPipelineStage stage,
                              const TechniqueParser::StageInfo& shader,
                              bool comments = false) = 0;

  static AfxCodegen* Create(const std::string& target_lan);
};

}  // namespace afx
}  // namespace azer
