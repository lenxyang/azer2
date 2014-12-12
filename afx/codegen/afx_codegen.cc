#include "azer/afx/codegen/afx_codegen.h"

#include "azer/afx/codegen/hlsl/afx_codegen.h"
#include "base/logging.h"

namespace azer {
namespace afx {
AfxCodegen* AfxCodegen::Create(const std::string& target_lan) {
  if (target_lan == "hlsl") {
    return new HLSLAfxCodegen();
  } else {
    CHECK(false) << "unsupport target language \"" << target_lan << "\"";
    return NULL;
  }
}

}  // namespace afx
}  // namespace azer
