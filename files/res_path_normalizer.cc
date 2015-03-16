#include "azer/files/res_path_normalizer.h"

#include "base/logging.h"
#include "base/strings/string_split.h"
#include "base/strings/string_tokenizer.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/base/string.h"
#include "azer/files/res_path_tokenizer.h"

namespace azer {
ResPathNormalizer::ResPathNormalizer() {
}

bool ResPathNormalizer::Normalize(ResPath* path) {
  using files::ResPathTokenizer;
  StringType pathstr = path->fullpath();
  path->clear();
  ResPathTokenizer tokenizer(pathstr);
  while (tokenizer.GetNext() != ResPathTokenizer::kSuccess) {
    const StringType& token = tokenizer.token();
    ResPathTokenizer::TokenType type = tokenizer.type();
    switch (type) {
      case ResPathTokenizer::kComponent:
        if (path->component_.empty()) {
          path->compoennt_ = token;
          path->fullpath_->append(token);
        } else {
        }
        break;
      case ResPathTokenizer::kDirSplitter:
        path->fullpath_.append(FILE_PATH_LITERAL("/"));
        break;
      case ResPathTokenizer::kRoot:
      case ResPathTokenizer::kDots:
      case ResPathTokenizer::kName:
        path->fullpath_.append(token);
        break;
    }
  }

  return true;
}

}  // namespace azer
