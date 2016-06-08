#pragma once

#include <string>
#include <vector>


#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "azer/base/image.h"

namespace azer {

ImageDataPtr LoadDDSImage(const base::FilePath& path);
ImageDataPtr LoadDDSImageFromMemory(const uint8_t* data, int32_t length);
}  // namespace azer
