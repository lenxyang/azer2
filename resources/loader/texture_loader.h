#pragma once

#include "azer/base/resources.h"
#include "azer/resources/resource_loader.h"

namespace azer {
class AZER_EXPORT DDSTextureLoader : public ResourceLoader {
 public:
  DDSTextureLoader();
  ~DDSTextureLoader() override;

  ResourcePtr LoadResource(const ResFilePath& path,
                           const FileContent* content) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(DDSTextureLoader);
};
}  // namespace azer
