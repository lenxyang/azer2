#include "azer/afx/linker/afx_facade.h"

#include "base/files/file_util.h"
#include "base/files/file_path.h"
#include "azer/afx/util/file_loader.h"

namespace azer {
namespace afx {
bool AfxFacade::Parse(const ::base::FilePath& path) {
  std::string content;
  DCHECK(initializer_ == false);
  if (!LoadFileToString(path, &content)) {
    PLOG(ERROR) << "cannot read file: " << path.value();
    return false;
  }

  return Parse(content, path);
}

bool AfxFacade::Parse(const std::string& content, const ::base::FilePath& path) {
  initializer_ = true;
  if (!linker_.Load(content, path)) {
    PLOG(ERROR) << "cannot read file: " << path.value();
    return false;
  }

  ParseContext* context = linker_.root()->GetContext();
  if (!context->success()) {
	return false;
  }
  if (!tlinker_.Parse(context)) {
    return false;
  }

  return true;
}

Technique* AfxFacade::GetTechnique(const std::string& name) {
  Technique* tech = NULL;
  if (tlinker_.GetTechnique(name, &tech)) {
    return tech;
  } else {
    return NULL;
  }
}

ParseContext* AfxFacade::GetContext() {
  DCHECK(linker_.root() != NULL);
  return linker_.root()->GetContext();
}
}  // namespace afx
}  // namespace azer
