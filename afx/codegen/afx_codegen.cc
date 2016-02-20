#include "azer/afx/codegen/afx_codegen.h"

#include "azer/afx/codegen/hlsl/afx_codegen.h"
#include "base/logging.h"

namespace azer {
namespace afx {
AfxCodegen* AfxCodegen::Create(const std::string& target_lan,
                               RenderPipelineStage stage) {
  if (target_lan == "hlsl") {
    switch (stage) {
      case kVertexStage:
        return new HLSLVSAfxCodegen();
      case kPixelStage:
        return new HLSLPSAfxCodegen();
	  case kGeometryStage:
		return new HLSLGSAfxCodegen();
      default:
        NOTREACHED() << "unsupport";
        return NULL;
    }
  } else {
    CHECK(false) << "unsupport target language \"" << target_lan << "\"";
    return NULL;
  }
}

}  // namespace afx
}  // namespace azer
