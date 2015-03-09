#pragma once

#include "azer/base/resources.h"
#include "azer/resources/resource_loader.h"

namespace azer {
class AZER_EXPORT VertexDescLoader : public ResourceLoader {
 public:
  VertexDescLoader();
  ~VertexDescLoader() override;

  ResourcePtr LoadResource(const ResFilePath& path,
                           const FileContent* content) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(VertexDescLoader);
};
}  // namespace azer
