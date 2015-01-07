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

Canvas2D* SoftwareContext2D::CreateCanvas(int32 width, int32 height) {
  return new SoftwareCanvas2D(width, height, this);
}

}  // namespace azer
