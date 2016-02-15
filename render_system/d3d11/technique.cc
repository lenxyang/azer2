#include "azer/render_system/d3d11/technique.h"
#include "azer/render_system/d3d11/shader.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
void D3DTechnique::Use(Renderer* renderer) {
  D3DVertexShader* vs = (D3DVertexShader*)pline_[kVertexStage].get();
  D3DPixelShader* ps = (D3DPixelShader*)pline_[kPixelStage].get();
  D3DGeometryShader* gs = (D3DGeometryShader*)pline_[kGeometryStage].get();
  D3DHullShader* hs = (D3DHullShader*)pline_[kHullStage].get();
  D3DDomainShader* ds = (D3DDomainShader*)pline_[kDomainStage].get();

  ID3D11DeviceContext* d3d_context = ((D3DRenderer*)renderer)->GetContext();
  if (vs) {
    DCHECK_EQ(vs->stage(), kVertexStage);
    d3d_context->VSSetShader(vs->resource(), 0, 0);
  } else {
    d3d_context->VSSetShader(NULL, 0, 0);
  }
  if (ps) {
    DCHECK_EQ(ps->stage(), kPixelStage);
    d3d_context->PSSetShader(ps->resource(), 0, 0);
  } else {
    d3d_context->PSSetShader(NULL, 0, 0);
  }

  if (gs) {
    DCHECK(gs->resource() != NULL);
    DCHECK_EQ(gs->stage(), kGeometryStage);
    d3d_context->GSSetShader(gs->resource(), 0, 0);
  } else {
    d3d_context->GSSetShader(NULL, 0, 0);
  }

  if (hs) {
    DCHECK(hs->resource() != NULL);
    DCHECK_EQ(hs->stage(), kHullStage);
    d3d_context->HSSetShader(hs->resource(), 0, 0);
  } else {
    d3d_context->HSSetShader(NULL, 0, 0);
  }

  if (ds) {
    DCHECK(ds->resource() != NULL);
    DCHECK_EQ(ds->stage(), kDomainStage);
    d3d_context->DSSetShader(ds->resource(), 0, 0);
  } else {
    d3d_context->DSSetShader(NULL, 0, 0);
  }
  // TODO(yanglei)
}
}  // namespace d3d11
}  // namespace azer
