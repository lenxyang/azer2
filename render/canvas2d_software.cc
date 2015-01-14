#include "azer/render/canvas2d_software.h"

#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkPicture.h"
#include "third_party/skia/include/core/SkImageInfo.h"
#include "third_party/skia/include/core/SkString.h"

#include "base/logging.h"
#include "azer/render/render_system.h"

namespace azer {
SoftwareCanvas2D::SoftwareCanvas2D(int width, int height, Context2D* context) 
    : Canvas2D(width, height, context) {
}

SoftwareCanvas2D::~SoftwareCanvas2D() {
  if (skcanvas_) {
    delete skcanvas_;
  }
}

bool SoftwareCanvas2D::Init() {
  // SkColorType ct = kRGBA_8888_SkColorType; not valid
  SkColorType ct = kN32_SkColorType;
  SkAlphaType at = kOpaque_SkAlphaType;
  SkImageInfo info = SkImageInfo::Make(width(), height(), ct, at);
  skbitmap_.reset(new SkBitmap());
  skbitmap_->allocPixels(info, 4 * width());
  skcanvas_ = new SkCanvas(*skbitmap_.get());
  return InitTexture();
}

uint32 SoftwareCanvas2D::GetTexID() {
  return static_cast<uint32>(-1);
}

bool SoftwareCanvas2D::UpdateTexture() {
  DCHECK(NULL != texture_.get());
  Texture::MapData mapdata = texture_->map(kWriteDiscard);
  if (mapdata.pdata) {
    /*
    skbitmap_->readPixels(skbitmap_->info(),
                          mapdata.pdata, 
                          mapdata.row_pitch, 0, 0);
    */
    skcanvas_->readPixels(skbitmap_->info(),
                          mapdata.pdata, 
                          mapdata.row_pitch, 0, 0);
    
  }
  texture_->unmap();
  return true;
}

bool SoftwareCanvas2D::InitTexture() {
  Texture::Options opt;
  opt.target = azer::Texture::kShaderResource;
  opt.size = gfx::Size(width(), height());
  opt.usage = GraphicBuffer::kDynamic;
  opt.cpu_access = kCPUWrite;
  RenderSystem* rs = RenderSystem::Current();
  texture_.reset(rs->CreateTexture(opt));
  return texture_.get() != NULL;
}

SkCanvas* SoftwareCanvas2D::BeginPaint() {
  return skcanvas_;
}

void SoftwareCanvas2D::EndPaint() {
  UpdateTexture();
}
}  // namespace azer
