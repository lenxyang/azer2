#pragma once

#include <memory>
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "azer/render/render.h"
#include "azer/base/image.h"

namespace azer {
AZER_EXPORT Vector4 SampleImage(float u, float v, Image* image);

// load image from file
AZER_EXPORT Image* LoadImageFromFile(const ::base::FilePath& path);

inline Image* LoadImageFromFile(const ::base::FilePath::StringType& path) {
  return LoadImageFromFile(::base::FilePath(path));
}

AZER_EXPORT ImageData* LoadImageData(const ::base::FilePath& path);

inline ImageData* LoadImageData(const ::base::FilePath::StringType& path) {
  return LoadImageData(::base::FilePath(path));
}

// create texture for shader resource
AZER_EXPORT Texture* CreateShaderTexture(const ::base::FilePath& path,
                                         azer::RenderSystem* rs);
inline Texture* CreateShaderTexture(const ::base::FilePath::StringType& path,
                                    azer::RenderSystem* rs) {
  return CreateShaderTexture(::base::FilePath(path), rs);
}

bool AZER_EXPORT SaveImage(ImageData* image, const ::base::FilePath& path);
}  // namespace azer
