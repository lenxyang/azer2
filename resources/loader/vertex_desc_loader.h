#pragma once

#include "azer/base/resources.h"
#include "azer/resources/resource_loader.h"

namespace azer {

class RepositoryNode;

class AZER_EXPORT VertexDescLoader : public ResourceLoader {
 public:
  VertexDescLoader();
  ~VertexDescLoader() override;

  bool LoadResource(const ResFilePath& path,
                    RepositoryNodePtr parent,
                    const FileContent* content) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(VertexDescLoader);
};
}  // namespace azer
