#pragma once

#include <string>
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/render/renderer.h"
#include "azer/render/movable.h"
#include "azer/render/camera.h"

namespace azer {
class AZER_EXPORT Light : public MovableObject,
                          public ::base::RefCounted<Light> {
 public:
  explicit Light(int32 id);
  virtual ~Light() {}

  int32 id() const { return id_;}

  /**
   * 光照 renderer
   */
  RendererPtr GetShadowRenderer();
  TexturePtr GetShadowMap();

  /**
   * 获得以光照为视角的 camera
   */
  const Camera* GetLightView() const { return &camera_;}
 protected:
  void OnPositionChanged(const Vector3& origin_position) override;
  void OnOrientationChanged(const Quaternion& origin_orientation) override;
  int32 id_;
  RendererPtr renderer_;
  Camera camera_;
  DISALLOW_COPY_AND_ASSIGN(Light);
};

typedef scoped_refptr<Light> LightPtr;
}  // namespace azer
