#include "azer/render_system/d3d11/technique.h"
#include "azer/render_system/d3d11/gpu_program.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {
void D3DTechnique::Use(Renderer* renderer) {
  D3DVertexGpuProgram* vs = (D3DVertexGpuProgram*)pline_[kVertexStage].get();
  D3DPixelGpuProgram* ps = (D3DPixelGpuProgram*)pline_[kPixelStage].get();
  D3DGeometryGpuProgram* gs = (D3DGeometryGpuProgram*)pline_[kGeometryStage].get();
  D3DHullGpuProgram* hs = (D3DHullGpuProgram*)pline_[kHullStage].get();
  D3DDomainGpuProgram* ds = (D3DDomainGpuProgram*)pline_[kDomainStage].get();
  DCHECK(vs != NULL);
  DCHECK(ps != NULL);
  DCHECK_EQ(vs->stage(), kVertexStage);
  DCHECK_EQ(ps->stage(), kPixelStage);

  ID3D11DeviceContext* d3d_context = ((D3DRenderer*)renderer)->GetContext();

  DCHECK(vs->input_layout_ != NULL);
  d3d_context->IASetInputLayout(vs->input_layout_);
  d3d_context->VSSetShader(vs->shader_, 0, 0);
  d3d_context->PSSetShader(ps->shader_, 0, 0);

  if (gs) {
    DCHECK(gs->shader_ != NULL);
    DCHECK_EQ(gs->stage(), kGeometryStage);
    d3d_context->GSSetShader(gs->shader_, 0, 0);
  } else {
    d3d_context->GSSetShader(NULL, 0, 0);
  }

  if (hs) {
    DCHECK(hs->shader_ != NULL);
    DCHECK_EQ(hs->stage(), kHullStage);
    d3d_context->HSSetShader(hs->shader_, 0, 0);
  } else {
    d3d_context->HSSetShader(NULL, 0, 0);
  }

  if (ds) {
    DCHECK(ds->shader_ != NULL);
    DCHECK_EQ(ds->stage(), kDomainStage);
    d3d_context->DSSetShader(ds->shader_, 0, 0);
  } else {
    d3d_context->DSSetShader(NULL, 0, 0);
  }
  // TODO(yanglei)
}
}  // namespace d3d11
}  // namespace azer
