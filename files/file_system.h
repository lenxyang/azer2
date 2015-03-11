#pragma once

#include <memory>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "azer/base/export.h"
#include "azer/base/string.h"
#include "azer/files/file_path.h"
#include "azer/files/file_content.h"

namespace azer {
class AZER_EXPORT FileSystem {
 public:
  virtual ~FileSystem() {}

  enum Type {
    kNativeFS = 1,
    kPackaged,
    kCompressedPackaged,
  };

  static FileSystem* create(Type type, const ::base::FilePath& root);
  const ::base::FilePath& root() { return fs_root_;}

  virtual FileContentPtr LoadFile(const FilePath& path) = 0;
  virtual bool IsPathExists(const FilePath& path) = 0;

  // Load content async
  // virtual ResLoadFileAsync(const ResFilePath& path, FileContent* filecontent) = 0;
 protected:
  FileSystem(Type type, const ::base::FilePath& root)
      : type_(type)
      , fs_root_(root) {}

  const Type type_;
  const ::base::FilePath fs_root_;

  DISALLOW_COPY_AND_ASSIGN(FileSystem);
};

typedef std::shared_ptr<FileSystem> FileSystemPtr;
}  // namespace azer
