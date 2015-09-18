#include "azer/render_system/d3d11/shader/coordinate_effect.h"

#include "azer/render/util/coordinate_grid_effect.h"

namespace azer {
namespace d3d11 {

const char* coordinate_effect_hlsl_vs = ""
    "#pragma pack_matrix(row_major)\n"
    "struct VsOutput {\n"
    "  float4 position:SV_POSITION;"
    "};\n"
    "struct VSInput {\n"
    "  float4 position:POSITION;\n"
    "};\n"
    "cbuffer c_buffer {\n"
    "  float4x4 pvw;\n"
    "};\n"
    "VsOutput vs_main(VSInput input) {\n"
    "  VsOutput o;\n"
    "  o.position = mul(pvw, input.position);\n"
    "  return o;\n"
    "}";

const char* coordinate_effect_hlsl_ps = ""
    "#pragma pack_matrix(row_major)\n"
    "struct VsOutput {\n"
    "  float4 position:SV_POSITION;"
    "};\n"
    "cbuffer c_buffer {\n"
    "  float4 color;\n"
    "};\n"
    "float4 ps_main(VsOutput o):SV_TARGET {"
    "  return color;\n"
    "}";

}  // namespace d3d11
}  // namespace azer
