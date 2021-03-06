#pragma once

#include "azer/render/indices_buffer.h"
#include "azer/render/blending.h"
#include "azer/render/depth_buffer.h"
#include "azer/render/common.h"
#include "azer/render/render_target.h"
#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render_system/d3d11/d3d11.h"

namespace azer {
namespace d3d11 {

D3D11_USAGE TranslateUsage(BufferUsage usage);
D3D11_CPU_ACCESS_FLAG TranslateCPUAccess(CPUAccess access);
DXGI_FORMAT TranslateFormat(DataFormat type);
DXGI_FORMAT TranslateTexFormat(TexFormat type);

DXGI_FORMAT TranslateIndexType(IndexType type);

D3D11_BIND_FLAG TranslateBindTarget(uint32_t target);

D3D11_TEXTURE_ADDRESS_MODE TranslateTexWrapMode(TexAddressMode mode);

D3D11_BLEND TranslateBlending(Blending::Blend type);
D3D11_BLEND_OP TranslateBlendingOper(Blending::Oper oper);
D3D11_COLOR_WRITE_ENABLE TranslateWriteMask(Blending::WriteMask mask);
D3D11_SRV_DIMENSION  GetViewDimensionFromTextureType(TexType type);
D3D11_MAP TranslateMapType(MapType type);
D3D11_COMPARISON_FUNC TranslateCompareFunc(CompareFunc type);
D3D11_STENCIL_OP TranslateStencilOper(StencilOper oper);
}  // namespace d3d11
}  // namespace azer
