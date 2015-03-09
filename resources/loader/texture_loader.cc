#include "azer/resources/loader/texture_loader.h"

namespace azer {
DDSTextureLoader::DDSTextureLoader() 
    : ResourceLoader(kTexture) {
}

DDSTextureLoader::~DDSTextureLoader() {
}

ResourcePtr DDSTextureLoader::LoadResource(const ResFilePath& path,
                                           const FileContent* content) {
}
}  // namespace azer
