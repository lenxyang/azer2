#include "azer/render/canvas2d_software.h"

#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkPicture.h"

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
  skbitmap_.reset(new SkBitmap());
  skbitmap_->allocN32Pixels(width(), height(), true);
  skcanvas_ = new SkCanvas(*skbitmap_.get());
  return InitTexture();
}

uint32 SoftwareCanvas2D::GetTexID() {
  return -1;
}

bool SoftwareCanvas2D::UpdateTexture() {
  DCHECK(NULL != texture_.get());
  Texture::MapData mapdata = texture_->map(kWriteDiscard);
  if (mapdata.pdata) {
    skbitmap_->readPixels(skbitmap_->info(),
                          mapdata.pdata, 
                          mapdata.row_pitch, 0, 0);
  }
  texture_->unmap();
  return true;
}

bool SoftwareCanvas2D::InitTexture() {
  Texture::Options opt;
  opt.target = azer::Texture::kShaderResource;
  opt.width = width();
  opt.height = height();
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
