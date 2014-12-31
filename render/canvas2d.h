#pragma once

#include <memory>

#include "base/basictypes.h"
#include "azer/render/render_system.h"

class SkCanvas;

namespace azer {

class Context2D;
class Device2D;

class AZER_EXPORT Canvas2D {
 public:
  ~Canvas2D();
  TexturePtr& GetTexture();

  int width() const { return width_;}
  int height() const { return height_;}

  void flush();

  bool Save(const ::base::FilePath& path);
  SkCanvas* GetSkCanvas();
 private:
  // create by canvas
  Canvas2D(int width, int height, Context2D* context);
  bool Init();

  TexturePtr texture_;
  int32 width_;
  int32 height_;
  Device2D* device_;
  Context2D* context_;
  friend class Context2D;
  DISALLOW_COPY_AND_ASSIGN(Canvas2D);
};

typedef std::shared_ptr<Canvas2D> Canvas2DPtr;


}  // namespace azer
