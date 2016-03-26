#pragma once

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/geometry/rect.h"
#include "azer/math/matrix4.h"
#include "azer/render/viewport.h"

namespace azer {
class AxesFrame;
class Camera;
class Overlay;
class Renderer;

class CameraOverlay {
 public:
  CameraOverlay(const Camera* camera);
  ~CameraOverlay();

  void SetBounds(const gfx::RectF& bounds);
  void Update();
  void Render(Renderer* renderer);
 private:
  Matrix4 world_;
  scoped_refptr<Overlay> overlay_;
  scoped_refptr<Renderer> renderer_;
  Viewport viewport_;
  scoped_ptr<AxesFrame> object_;
  const Camera* camera_;
  DISALLOW_COPY_AND_ASSIGN(CameraOverlay);
};

}  // namespace azer
