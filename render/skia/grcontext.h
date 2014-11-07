#pragma once

#include "gl/SkGLContext.h"
#include "base/basictypes.h"
#include "azer/render/render_system.h"
#include "azer/render/egl.h"
#include "base/logging.h"

struct GrGLInterface;

namespace azer {
namespace skia {

class ASkGLContext : public SkGLContext {
 public:
  ASkGLContext(int width, int height);
  virtual ~ASkGLContext();

  virtual void makeCurrent() const override;
  virtual void swapBuffers() const override;


  const GrGLInterface* GetGrGlInterface() {
    DCHECK(fGL.get() != NULL);
    return fGL.get();
  }
  AzerEGLContext* GetAzerEGLContext() { return &context_;}
  EGL* GetEGL() { return azer_egl_;}
 protected:
  const GrGLInterface* createGLContext();
  void destroyGLContext();
 private:
  // AutoContextRestore auto_restore_;
  AzerEGLContext context_;
  EGL* azer_egl_;
  int width_, height_;
  DISALLOW_COPY_AND_ASSIGN(ASkGLContext);
};
}  // namespace skia
}  // namespace azer
