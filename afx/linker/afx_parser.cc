#include "azer/afx/linker/afx_parser.h"

#include "base/files/file_util.h"
#include "base/files/file_path.h"
#include "azer/afx/util/file_loader.h"

namespace azer {
namespace afx {
bool AfxParser::Parse(const ::base::FilePath& path) {
  std::string content;
  DCHECK(initializer_ == false);
  if (!LoadFileToString(path, &content)) {
    PLOG(ERROR) << "cannot read file: " << path.value();
    return false;
  }

  return Parse(content, path);
}

bool AfxParser::Parse(const std::string& content, const ::base::FilePath& path) {
  initializer_ = true;
  if (!linker_.Load(content, path)) {
    PLOG(ERROR) << "cannot read file: " << path.value();
    return false;
  }

  ParseContext* context = linker_.root()->GetContext();
  if (!tparser_.Parse(context)) {
    return false;
  }

  return true;
}

TechniqueParser::Technique* AfxParser::GetTechnique(const std::string& name) {
  TechniqueParser::Technique* tech = NULL;
  if (tparser_.GetTechnique(name, &tech)) {
    return tech;
  } else {
    return NULL;
  }
}

ParseContext* AfxParser::GetContext() {
  DCHECK(linker_.root() != NULL);
  return linker_.root()->GetContext();
}
}  // namespace afx
}  // namespace azer
