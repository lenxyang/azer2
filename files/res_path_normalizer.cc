#include "azer/files/res_path_normalizer.h"

#include "base/logging.h"
#include "base/strings/string_split.h"
#include "base/strings/string_tokenizer.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/base/string.h"
#include "azer/files/res_path_parser.h"

namespace azer {

ResPathNormalizer::ResPathNormalizer() {
}

bool ResPathNormalizer::Normalize(ResPath* path) {
  using files::ResPathParser;
  StringType pathstr = path->fullpath();
  path->clear();
  ResPathParser tokenizer(pathstr);
  StringType component;
  std::vector<StringType> vec;
  while (tokenizer.GetNext() != ResPathParser::kSuccess) {
    const StringType& token = tokenizer.token();
    ResPathParser::Type type = tokenizer.type();
    switch (type) {
      case ResPathParser::kComponent:
        component = token;
        break;
      case ResPathParser::kPrevDir:
        if (!component.empty()) return false;
        if (vec.empty()) return false;
        if (vec.back() == FILE_PATH_LITERAL("//")) return false;
        vec.resize(vec.size() - 1);
        break;
      case ResPathParser::kDirSplitter:
      case ResPathParser::kCurrentDir:
        if (!component.empty()) return false;
        break;
      case ResPathParser::kRoot:
        if (!component.empty()) return false;
        vec.push_back(FILE_PATH_LITERAL("//"));
        break;
      case ResPathParser::kName:
        if (!component.empty()) return false;
        vec.push_back(token);
        break;
    }
  }

  return true;
}

}  // namespace azer
