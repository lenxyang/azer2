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
  StringType component;
  std::vector<StringType> vec;
  while (tokenizer.GetNext() != ResPathTokenizer::kSuccess) {
    const StringType& token = tokenizer.token();
    ResPathTokenizer::TokenType type = tokenizer.type();
    switch (type) {
      case ResPathTokenizer::kComponent:
        component = token;
        break;
      case ResPathTokenizer::kPrevDir:
        if (!component.empty()) return false;
        if (vec.empty()) return false;
        if (vec.back() == FILE_PATH_LITERAL("//")) return false;
        vec.resize(vec.size() - 1);
        break;
      case ResPathTokenizer::kDirSplitter:
      case ResPathTokenizer::kCurrentDir:
        if (!component.empty()) return false;
        break;
      case ResPathTokenizer::kRoot:
        if (!component.empty()) return false;
        vec.push_back(FILE_PATH_LITERAL("//"));
        break;
      case ResPathTokenizer::kName:
        if (!component.empty()) return false;
        vec.push_back(token);
        break;
    }
  }

  return true;
}

}  // namespace azer
