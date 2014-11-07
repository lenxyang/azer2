#include "azer/afx/util/file_loader.h"

#include "azer/base/string.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/strings/utf_string_conversions.h"

namespace azer {
namespace afx {
bool FileLoader::Load(const ::base::FilePath& file, std::string* content,
                      ::base::FilePath* finalpath) {
  for (auto iter = include_dirs_.begin(); iter != include_dirs_.end();
       ++iter) {
    ::base::FilePath fullpath(*iter);
    fullpath = fullpath.Append(file);
    if (LoadFileToString(fullpath, content)) {
      *finalpath = fullpath.NormalizePathSeparators();
      return true;
    }
  }

  ::base::FilePath fullpath(file);
  if (LoadFileToString(fullpath, content)) {
    *finalpath = fullpath;
    return true;
  } else {
    return false;
  }
}

bool FileLoader::LoadFileToString(const ::base::FilePath& path,
                                  std::string* content) {
  if (! ::base::PathExists(path)) {
    return false;
  }

  return ::base::ReadFileToString(path, content);
}

bool MemoryFileLoader::LoadFileToString(const ::base::FilePath& path,
                                        std::string* content) {
  auto iter = content_.find(path.value());
  if (iter != content_.end()) {
    *content = iter->second;
    return true;
  } else {
    return false;
  }
}

bool LoadFileToString(const ::base::FilePath& path, std::string* content,
                      int max_size) {
  CHECK(content != NULL);
  content->clear();
  FILE* file = ::base::OpenFile(path, "rb");
  if (!file) {
    return false;
  }

  const size_t kBufferSize = 1 << 16;
  scoped_ptr<char[]> buf(new char[kBufferSize]);
  size_t len;
  size_t size = 0;
  bool read_status = true;

  // Many files supplied in |path| have incorrect size (proc files etc).
  // Hence, the file is read sequentially as opposed to a one-shot read.
  while ((len = fread(buf.get(), 1, kBufferSize, file)) > 0) {
    if (content)
      content->append(buf.get(), std::min(len, max_size - size));

    if ((max_size - size) < len) {
      read_status = false;
      break;
    }

    size += len;
  }
  read_status = read_status && !ferror(file);
  ::base::CloseFile(file);

  return read_status;
}
}  // namespace afx
}  // namespace azer
