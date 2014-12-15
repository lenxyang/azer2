#pragma once

#include "base/basictypes.h"
#include "azer/afx/linker/technique.h"

namespace azer {
namespace afx {

class TechniqueParser;

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
  TechniqueValidator(TechniqueParser* parser);
  bool Valid(const Technique& technique);
 private:
  bool ValidGeometryShader(const Technique::StageInfo& shader);

  TechniqueParser* parser_;
  DISALLOW_COPY_AND_ASSIGN(TechniqueValidator);
};

inline TechniqueValidator::TechniqueValidator(TechniqueParser* parser)
    : parser_(parser) {
}
}  // namespace afx
}  // namespace azer
