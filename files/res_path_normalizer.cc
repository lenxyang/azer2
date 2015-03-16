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
  StringType pathstr = path->rawpath();
  path->clear();
  path->type_ = ResPath::kRelativePath;
  path->rawpath_ = pathstr;
  ResPathParser tokenizer(pathstr);
  std::vector<StringType> vec;
  while (tokenizer.GetNext() != ResPathParser::kSuccess) {
    const StringType& token = tokenizer.token();
    ResPathParser::Type type = tokenizer.type();
    switch (type) {
      case ResPathParser::kProtoSpecifier:
        path->proto_ = token;
        path->type_ = ResPath::kProtoPath;
        break;
      case ResPathParser::kComponent:
        path->component_ = token;
        break;
      case ResPathParser::kPrevDir:
        if (vec.empty()) return false;
        if (vec.back() == FILE_PATH_LITERAL("//")) return false;
        vec.resize(vec.size() - 1);
        break;
      case ResPathParser::kDirSplitter:
      case ResPathParser::kCurrentDir:
        break;
      case ResPathParser::kRoot:
        vec.push_back(FILE_PATH_LITERAL("//"));
        path->type_ = ResPath::kAbsolutePath;
        break;
      case ResPathParser::kName:
        vec.push_back(token);
        break;
      case ResPathParser::kErrorToken:
        path->type_ = ResPath::kInvalidPath;
        return false;
      default:
        CHECK(false);
        return false;
    }
  }

  path->fullpath_.append(path->proto());
  for (size_t i = 0; i < vec.size(); ++i) {
    if (i > 1) {
      path->fullpath_.append(FILE_PATH_LITERAL("/"));
    } else if (i == 1 && path->type() != ResPath::kAbsolutePath) {
      path->fullpath_.append(FILE_PATH_LITERAL("/"));
    }

    path->fullpath_.append(vec[i]);
  }

  path->file_path_ = path->fullpath_;
  path->fullpath_.append(path->component());
  return true;
}

}  // namespace azer
