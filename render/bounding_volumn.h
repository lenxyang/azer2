#pragma once

#include <memory>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/render_system_enum.h"

namespace azer {

class Camera;
class Renderer;
class Matrix4;

class Frustrum;
class AZER_EXPORT BoundingVolumn : public ::base::RefCounted<BoundingVolumn> {
 public:
  enum Type {
    kNoBoundingVolumn,
    kSphere,
    kAxisAlignedBox,
    kOrientedBox,
  };

  virtual VisibleState IsVisible(const Frustrum& frustrum, const Matrix4& world) = 0;
  virtual void Render(Renderer* renderer, const Matrix4& world,
                      const Camera& camera) = 0;
};

typedef scoped_refptr<BoundingVolumn> BoundingVolumnPtr;
}  // namespace azer
