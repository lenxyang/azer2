#pragma once

#include "base/basictypes.h"
#include "azer/afx/linker/technique_parser.h"

namespace azer {
namespace afx {
class TechniqueValidator {
 public:
  bool Valid(const Technique& technique);
 private:
  DISALLOW_COPY_AND_ASSIGN(TechniqueValidator);
};
}  // namespace afx
}  // namespace azer
