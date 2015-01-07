#pragma once

#include <memory>

#include "base/basictypes.h"
#include "azer/base/export.h"
#include "azer/render/texture.h"
#include "base/logging.h"

class SkCanvas;
class SkBitmap;

namespace azer {

class Context2D;
class Device2D;

class AZER_EXPORT Canvas2D {
 public:
  virtual ~Canvas2D();

  int32 width() const { return width_;}
  int32 height() const { return height_;}

  // is use SkCanvas to Paint, should call the following functions
  virtual SkCanvas* BeginPaint() = 0;
  virtual void EndPaint() = 0;

  SkCanvas* GetSkCanvas() {
    DCHECK(NULL != skcanvas_);
    return skcanvas_;
  }

  Context2D* GetContext2D() {
    DCHECK(NULL != context_);
    return context_;
  }

  TexturePtr& GetTexture() { return texture_;}
  virtual uint32 GetTexID() = 0;

  // save the content into file
  bool Save(const ::base::FilePath& path);
 protected:
  // create by canvas
  Canvas2D(int32 width, int32 height, Context2D* context);
  virtual bool Init() = 0;
  // virtual TexturePtr CreateSharedTexture() = 0;
  
  const int32 width_;
  const int32 height_;
  TexturePtr texture_;
  SkCanvas* skcanvas_;
  Context2D* context_;
  friend class Context2D;
  DISALLOW_COPY_AND_ASSIGN(Canvas2D);
};

typedef std::shared_ptr<Canvas2D> Canvas2DPtr;

bool AZER_EXPORT SaveSkCanvas(SkCanvas* canvas, const ::base::FilePath& path);
bool AZER_EXPORT SaveSkBitmap(const SkBitmap& bitmap, const ::base::FilePath& path);
}  // namespace azer
