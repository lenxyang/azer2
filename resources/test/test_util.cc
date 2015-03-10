#include "azer/resources/test/test_util.h"

#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "base/strings/string_tokenizer.h"

namespace azer {
namespace test {


RepositoryNodePtr GenerateRepositoryTreeFromString(const std::string& str) {
  RepositoryNodePtr root;
  std::vector<StringType> lines;
  ::base::SplitString(str, '\n', &lines);
  for (auto iter = lines.begin(); iter != lines.end(); ++iter) {
    StringTokenizer t(*iter, "/");
    while (t.GetNext()) {
    }
  }

  return root;
}
}  // namespace test
}  // namespace azer
