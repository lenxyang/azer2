#pragma once

#include <memory>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/logging.h"
#include "azer/base/export.h"
#include "azer/render/texture.h"
#include "azer/render/texture_view.h"

class SkCanvas;
class SkBitmap;

namespace azer {

class Context2D;
class Device2D;

class AZER_EXPORT Canvas2D : public ::base::RefCounted<Canvas2D> {
 public:
  virtual ~Canvas2D();

  int32_t width() const { return width_;}
  int32_t height() const { return height_;}

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

  Texture* GetTexture() { return texture_;}
  TextureView* GetTextureView() { return texview_;}
  virtual uint32_t GetTexID() = 0;

  // save the content into file
  bool Save(const ::base::FilePath& path);
 protected:
  // create by canvas
  Canvas2D(int32_t width, int32_t height, Context2D* context);
  virtual bool Init() = 0;
  // virtual TexturePtr CreateSharedTexture() = 0;
  
  const int32_t width_;
  const int32_t height_;
  TexturePtr texture_;
  TextureViewPtr texview_;
  SkCanvas* skcanvas_;
  Context2D* context_;
  friend class Context2D;
  DISALLOW_COPY_AND_ASSIGN(Canvas2D);
};

typedef scoped_refptr<Canvas2D> Canvas2DPtr;

bool AZER_EXPORT SaveSkCanvas(SkCanvas* canvas, const ::base::FilePath& path);
bool AZER_EXPORT SaveSkBitmap(const SkBitmap& bitmap, const ::base::FilePath& path);
}  // namespace azer
