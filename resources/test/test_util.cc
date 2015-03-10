#include "azer/resources/test/test_util.h"

#include "base/files/file_path.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/strings/string_tokenizer.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/base/string.h"

namespace azer {
namespace test {

RepositoryNodePtr GenerateRepositoryTreeFromString(const std::string& str) {
  RepositoryNodePtr root(new RepositoryNode);
  std::vector<std::string> vec;
  ::base::SplitStringUsingSubstr(str, "//", &vec);
  for (auto iter = vec.begin() + 1; iter != vec.end(); ++iter) {
    std::string path = *iter;
    path.insert(0, "//");
    StringType fullpath;
    ConvertPath(path.c_str(), &fullpath);
    RepositoryNodePtr parent = root->GetNodeParent(fullpath);
    CHECK(parent.get());
    ::base::FilePath filepath(fullpath.substr(2));
    RepositoryNodePtr node(new RepositoryNode(filepath.BaseName().value()));
    parent->AddChild(node);
  }

  return root;
}
}  // namespace test
}  // namespace azer
