#pragma once

#include "azer/render/reusable_object.h"

namespace azer {

class RenderSystem;

namespace d3d11 {

class D3DReusableObject : public ReusableObject {
 public:
  D3DReusableObject();
  ~D3DReusableObject();

  bool Init(RenderSystem* rs) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(D3DReusableObject);
};
}  // namespace d3d11
}  // namespace azer
