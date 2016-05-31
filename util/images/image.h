#pragma once

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/ref_counted.h"
#include "azer/base/image.h"

namespace azer {

ImageDataPtr LoadDDSImage(const base::FilePath& path);
ImageDataPtr LoadDDSImageFromMemory(const uint8* data, int32_t length);
}  // namespace azer
