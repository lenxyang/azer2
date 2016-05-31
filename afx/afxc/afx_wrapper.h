#pragma once

#include <memory>
#include <vector>
#include <string>


#include "azer/afx/linker/afx_facade.h"

namespace azer {
namespace afx {

class AfxWrapper {
 public:
  explicit AfxWrapper(const ::base::FilePath::StringType& includes);

  struct AfxResult {
    std::string cpp;
    std::string hpp;
    std::vector<std::string> hlsl;
    std::vector<std::string> glsl;
    const Technique* technique;
  };
  bool Parse(const ::base::FilePath& path, std::string* str,
             std::vector<AfxResult>* result); 
 private:
  void GenCppCode(const Technique& tech, AfxResult* result);
  void GenHLSL(const Technique& tech, AfxResult* result);
  std::vector< ::base::FilePath::StringType> includes_;
  std::unique_ptr<AfxFacade> facade_;
  DISALLOW_COPY_AND_ASSIGN(AfxWrapper);
};

typedef std::vector<AfxWrapper::AfxResult> AfxWrapperResultVec;
} // namespace afx
}  // namespace azer
