#include "azer/util/interactive/axes_frame.h"

#include "base/logging.h"
#include "azer/effect/effect.h"
#include "azer/render/vertex_pack.h"

namespace azer {
AxesFrame::AxesFrame() {
  EntityDataPtr data(new EntitydData(effect_->vertex_desc()));
  VertexPack vpack(data->vdata());
  
  entity_ = new Entity(data);
}

void AxesFrame::Update(const Camera* camera) {
}

void AxesFrame::SetColor(int32 index, const Vector4& color) {
  DCHECK(index, static_cast<int32>(arraysize(colors_)));
  colors_[index] = color;
}

void AxesFrame::Render(Renderer* renderer) {
}
}  // namespace azer
