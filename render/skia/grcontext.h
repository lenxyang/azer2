#pragma once

#include "gl/SkGLContext.h"
#include "base/basictypes.h"
#include "azer/render/render_system.h"
#include "azer/render/glcontext.h"
#include "base/logging.h"

struct GrGLInterface;

namespace azer {
namespace skia {

class AzerSkiaGrContext : public SkGLContext {
 public:
  AzerSkiaGrContext(int width, int height);
  virtual ~AzerSkiaGrContext();

  virtual void makeCurrent() const override;
  virtual void swapBuffers() const override;


  const GrGLInterface* GetGrGlInterface() {
    DCHECK(fGL.get() != NULL);
    return fGL.get();
  }
  AzerEGLContext* GetAzerEGLContext() { return &context_;}
  AzerEGLInterface* GetAzerEGLInterface() { return azer_egl_;}
 protected:
  const GrGLInterface* createGLContext();
  void destroyGLContext();
 private:
  // AutoContextRestore auto_restore_;
  AzerEGLContext context_;
  AzerEGLInterface* azer_egl_;
  int width_, height_;
  DISALLOW_COPY_AND_ASSIGN(AzerSkiaGrContext);
};
}  // namespace skia
}  // namespace azer
