#include "azer/render/context2d_software.h"

#include "azer/render/canvas2d_software.h"

namespace azer {

SoftwareContext2D::SoftwareContext2D() {
}

SoftwareContext2D::~SoftwareContext2D() {
}
  
void SoftwareContext2D::flush() {
}

void SoftwareContext2D::finish() {
}

Canvas2D* SoftwareContext2D::CreateCanvas(int32_t width, int32_t height) {
  std::unique_ptr<SoftwareCanvas2D> ptr(new SoftwareCanvas2D(width, height, this));
  if (ptr->Init()) {
    return ptr.release();
  } else {
    return NULL;
  }
}

}  // namespace azer
