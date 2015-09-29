#pragma once

#include "azer/render/util/geometry/geometry_object.h"

namespace azer {

class BoxObject : public GeometryObject {
 public:
  BoxObject(PVWEffectPtr effect);
  ~BoxObject();

  void Render(Renderer* renderer) override;
 private:
  void InitHardwareBuffers();
  DISALLOW_COPY_AND_ASSIGN(BoxObject);
};
}  // namespace azer
