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
  ResPathTokenizer tokenizer(path->fullpath());
  while (tokenizer.GetNext() != ResPathTokenizer::kSuccess) {
    const StringType& token = tokenizer.token();
    CharType first = token[0];
    if (first == FILE_PATH_LITERAL(':')) {
    } else if (first == FILE_PATH_LITERAL('/')) {
    } else {
    }
  }

  return true;
}

}  // namespace azer
