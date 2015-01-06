#include "azer/render_system/d3d11/reusable_object.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {

D3DReusableObject::D3DReusableObject() {
}

D3DReusableObject::~D3DReusableObject() {
}

bool D3DReusableObject::Init(RenderSystem* rs) {
  return ReusableObject::Init(rs);
}
}  // namespace d3d11
}  // namespace azer
