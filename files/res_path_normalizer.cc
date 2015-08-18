#include "azer/files/res_path_normalizer.h"

#include "base/logging.h"
#include "base/strings/string_split.h"
#include "base/strings/string_tokenizer.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/base/string.h"
#include "azer/files/res_path_parser.h"

namespace azer {

ResPathNormalizer::ResPathNormalizer(const StringType& raw_path)
    : raw_path_(raw_path) {
}

bool ResPathNormalizer::Normalize(ResPath* path) {
  using files::ResPathParser;
  StringType proto;
  StringType component;
  StringType pathstr = raw_path_;
  path->clear();
  ResPath::PathType path_type = ResPath::kRelativePath;
  ResPathParser tokenizer(pathstr);
  std::vector<StringType> vec;
  while (tokenizer.GetNext() == ResPathParser::kSuccess) {
    const StringType& token = tokenizer.token();
    ResPathParser::Type type = tokenizer.type();
	if (type == ResPathParser::kEnd) break;
    switch (type) {
      case ResPathParser::kProtoSpecifier:
        proto = token;
        path_type = ResPath::kProtoPath;
        break;
      case ResPathParser::kComponent:
        component = token;
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
        path_type = ResPath::kAbsolutePath;
        break;
      case ResPathParser::kName:
        vec.push_back(token);
        break;
      case ResPathParser::kErrorToken:
        path_type = ResPath::kInvalidPath;
        return false;
      case ResPathParser::kEnd:
        break;
      default:
        CHECK(false);
        return false;
    }
  }

  StringType filepath;
  for (size_t i = 0; i < vec.size(); ++i) {
    if (i > 1) {
      filepath.append(FILE_PATH_LITERAL("/"));
    } else if (i == 1 && path_type != ResPath::kAbsolutePath) {
      filepath.append(FILE_PATH_LITERAL("/"));
    }

    filepath.append(vec[i]);
  }

  path->Init(proto, filepath, component, path_type);
  
  return true;
}

}  // namespace azer
