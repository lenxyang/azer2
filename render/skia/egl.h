#pragma once

#include "base/logging.h"
#include "azer/render/texture.h"

struct GrGLInterface;

namespace azer {

class EGL {
 public:
  struct AEGLContext {
    void* display;
    void* context;
    void* surface;
    void* user_data;
    AEGLContext()
        : display(NULL)
        , context(NULL)
        , surface(NULL)
        , user_data(NULL) {
    }
  };

  EGL() {}
  virtual ~EGL() {}
  virtual bool Init() = 0;
  virtual bool MakeCurrent() = 0;
  virtual void Destroy() = 0;
  virtual Texture* GetShareTextureFromTex(uint32 texid)  = 0;

  virtual void* GetProcAddress(const char* name) = 0;  
 protected:
  AEGLContext context_;
};
}  // namespace azer
