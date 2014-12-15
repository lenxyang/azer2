#pragma once

#include <string>
#include <vector>
#include "azer/afx/codegen/hlsl/afx_codegen.h"
#include "azer/afx/codegen/hlsl/ast_codegen.h"
#include "azer/afx/compiler/astdump.h"
#include "azer/afx/compiler/testutil.h"
#include "azer/afx/linker/technique.h"
#include "azer/afx/linker/technique_linker.h"
#include "azer/afx/linker/afx_linker.h"
#include "azer/afx/linker/testutil.h"
#include "azer/afx/util/file_loader.h"
#include "base/basictypes.h"
#include "base/logging.h"

namespace azer {
namespace afx {
class AfxParser {
 public:
  AfxParser(const std::vector< ::base::FilePath::StringType>& incpath,
            const AfxLinker::Options& link_opt)
      : initializer_(false)
      , loader_(incpath)
      , linker_(&loader_, link_opt) {
  }

  bool Parse(const ::base::FilePath& path);
  bool Parse(const ::base::FilePath::StringType& path);
  bool Parse(const std::string& content, const ::base::FilePath& path);
  Technique* GetTechnique(const std::string& name);
  TechniqueLinker* GetTechniques() { return &tlinker_;}

  ParseContext* GetContext();

  bool success() const {
    DCHECK(initializer_);
    return linker_.success();
  }

  std::string GetCompileError() const {
    return linker_.GetCompileError();
  }

  const std::string GetErrorText() const {
    return linker_.GetErrorText();
  }
 private:
  bool initializer_;
  FileLoader loader_;
  AfxLinker linker_;
  TechniqueLinker tlinker_;
  DISALLOW_COPY_AND_ASSIGN(AfxParser);
};

inline bool AfxParser::Parse(const ::base::FilePath::StringType& path) {
  return Parse(::base::FilePath(path));
}

}  // namespace afx
}  // namespace azer
