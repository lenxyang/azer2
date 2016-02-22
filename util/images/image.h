#pragma once

#include "azer/base/image.h"

namespace azer {
ImagePtr LoadDDSImage(const base::FilePath& path);
}  // namespace azer
