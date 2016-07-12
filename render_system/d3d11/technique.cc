#include "azer/render_system/d3d11/technique.h"
#include "azer/render_system/d3d11/shader.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/dx3d_util.h"

namespace azer {
namespace d3d11 {

TechniquePtr D3DTechnique::CopyTechnique() {
  scoped_refptr<D3DTechnique> ptr(new D3DTechnique());
  ptr->pline_ = pline_;
  return ptr;
}
}  // namespace d3d11
}  // namespace azer
