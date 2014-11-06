#pragma once

#include "gl/SkGLContext.h"
#include "base/basictypes.h"
#include "azer/render/render_system.h"
#include "azer/render/glcontext.h"

struct GrGLInterface;

namespace azer {
namespace skia {

class AzerSkiaGrContext : public SkGLContext {
 public:
  AzerSkiaGrContext(int width, int height);
  virtual ~AzerSkiaGrContext();

  virtual void makeCurrent() const override;
  virtual void swapBuffers() const override;

  AzerEGLContext* GetAzerEGLContext() { return &context_;}
  AzerEGLInterface* GetAzerEGLInterface() { return agl_interface_;}
 protected:
  const GrGLInterface* createGLContext();
  void destroyGLContext();
 private:
  // AutoContextRestore auto_restore_;
  AzerEGLContext context_;
  AzerEGLInterface* agl_interface_;
  int width_, height_;
  DISALLOW_COPY_AND_ASSIGN(AzerSkiaGrContext);
};
}  // namespace skia
}  // namespace azer
