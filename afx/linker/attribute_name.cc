#include "azer/afx/linker/attribute_name.h"

namespace azer {
namespace afx {

const std::string AttrNames::kGSMaxVertexCount = "max_vertex_count";
const std::string AttrNames::kGSVertexType = "vertex_type";
const std::string AttrNames::kGSInputPrimitiveType = "input_primitive_type";
const std::string AttrNames::kGSOutputPrimitiveType = "output_primitive_type";
const std::string AttrNames::kGSEmitVariable = "gs_emitted";

const std::string AttrNames::kName = "name";
const std::string AttrNames::kVertexShaderEntry = "vs_main";
const std::string AttrNames::kHullShaderEntry = "hs_main";
const std::string AttrNames::kDomainShaderEntry = "ds_main";
const std::string AttrNames::kGeometryShaderEntry = "gs_main";
const std::string AttrNames::kPixelShaderEntry = "ps_main";

bool AttrNames::ValidGSPrimitiveTypeValue(const std::string& val) {
  if (val == "ponit"
      || val == "line"
      || val == "triangle") {
    return true;
  } else {
    return false;
  }
}
}  // namespace afx
}  // namespace azer
