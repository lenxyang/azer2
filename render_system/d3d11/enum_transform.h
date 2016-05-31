#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

#include "azer/render/indices_buffer.h"
#include "azer/render/blending.h"
#include "azer/render/depth_buffer.h"
#include "azer/render/common.h"
#include "azer/render/render_target.h"
#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"


namespace azer {
namespace d3d11 {

D3D11_USAGE TranslateUsage(BufferUsage usage);
D3D11_CPU_ACCESS_FLAG TranslateCPUAccess(CPUAccess access);
DXGI_FORMAT TranslateFormat(DataFormat type);
DXGI_FORMAT TranslateTexFormat(TexFormat type);
DXGI_FORMAT TranslateTexResFormat(TexFormat type);

TexFormat TranslateD3DTexFormat(DXGI_FORMAT type);
DataFormat TranslateD3DFormat(DXGI_FORMAT type);

D3D_PRIMITIVE_TOPOLOGY TranslatePrimitiveTopology(PrimitiveTopology topology);

DXGI_FORMAT TranslateIndexType(IndexType type);

D3D11_BIND_FLAG TranslateBindTarget(uint32_t target);

D3D11_FILL_MODE TranslateFillMode(FillMode mode);
FillMode TranslateD3DFillMode(D3D11_FILL_MODE mode);

D3D11_CULL_MODE TranslateCullingMode(CullingMode mode);
CullingMode TranslateD3DCullingMode(D3D11_CULL_MODE mode);

D3D11_TEXTURE_ADDRESS_MODE TranslateTexWrapMode(AddressMode mode);

D3D11_BLEND TranslateBlending(Blending::Blend type);
D3D11_BLEND_OP TranslateBlendingOper(Blending::Oper oper);
D3D11_COLOR_WRITE_ENABLE TranslateWriteMask(Blending::WriteMask mask);
D3D11_SRV_DIMENSION  GetViewDimensionFromTextureType(TexType type);
D3D11_MAP TranslateMapType(MapType type);
D3D11_COMPARISON_FUNC TranslateCompareFunc(CompareFunc type);
D3D11_STENCIL_OP TranslateStencilOper(StencilOper oper);
D3D11_FILTER TranslateSamplerStateFilter(const SamplerState::Options& state);
D3D11_FILTER TranslateSamplerStateCompFilter(const SamplerState::Options& state);
}  // namespace d3d11
}  // namespace azer
