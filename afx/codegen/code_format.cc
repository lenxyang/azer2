#include "azer/afx/codegen/util.h"

#include "azer/afx/compiler/astnode.h"
#include "base/strings/string_util.h"
#include "base/strings/string_split.h"
#include "azer/afx/util/file_loader.h"
#include "base/files/file_util.h"
#include <sstream>

namespace azer {
namespace afx {
std::string FormatCode(const std::string& code) {
  std::stringstream out;
  int depth = 0;
  bool prev_new_line = false;
  const char* cur = code.c_str();
  while (*cur != '\0') {
    if (StartsWithASCII(cur, "};", true)) {
      out << "};\n";
      prev_new_line = true;
      cur += 2;
      depth--;
    } else if (StartsWithASCII(cur, ";\n", true)) {
      out << ";\n";
      prev_new_line = true;
      cur += 2;
    } else if (*cur == '}') {
      out << "}\n";
      prev_new_line = true;
      cur++;
      depth--;
    } else if (*cur == '{') {
      out << "{\n";
      depth++;
      prev_new_line = true;
      cur++;
    } else if (*cur == ';') {
      out << ";\n";
      prev_new_line = true;
      cur++;
    } else {
      if (prev_new_line) {
        out << std::string(depth * 2, ' ');
        prev_new_line = false;
      }
      out << *cur;
      cur++;
    }
  }

  return out.str();
}
}  // namespace afx
}  // namespace azer

bool CompareCode(const ::base::FilePath& path, const std::string& content) {
  std::string content1;
  if (!azer::afx::LoadFileToString(path, &content1)) {
    return false;
  }

  std::string str1, str2;
  ::base::ReplaceChars(content1, "\r\n", "", &str1);
  ::base::ReplaceChars(content, "\r\n", "", &str2);
  std::string s1, s2;
  ::base::TrimWhitespace(str1, ::base::TRIM_ALL, &s1);
  ::base::TrimWhitespace(str2, ::base::TRIM_ALL, &s2);
  if (s1 == s2) {
    return true;
  } else {
    LOG(ERROR) << "EXPECT(length: " << s1.length() << ")\n" << s1;
    LOG(ERROR) << "RESULT(length: " << s2.length() << ")\n" << s2;
    return false;
  }  
}
