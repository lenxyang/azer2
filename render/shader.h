#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/base/string.h"
#include "azer/render/common.h"

namespace azer {
class ResPath;
class FileSystem;
class RenderSystem;
class VertexDesc;
class TechSource;
class Shader;
typedef scoped_refptr<VertexDesc> VertexDescPtr;
typedef scoped_refptr<Shader> ShaderPtr;

enum ShaderType {
  kStringShader,
  kBinaryShader,
};

struct AZER_EXPORT ShaderInfo {
  ShaderInfo();
  int32 stage;
  int32 format;
  std::string code;
  std::string path;
  std::string version;
  std::string entry;
  std::string defines;
  std::string includes;
};

class AZER_EXPORT Shader : public ::base::RefCounted<Shader> {
 public:
  explicit Shader(const ShaderInfo& info);
  Shader(VertexDesc* desc, const ShaderInfo& info);
  virtual ~Shader();

  std::string error_msg() const { return error_msg_;}
  RenderPipelineStage stage() const;

  virtual bool Init(RenderSystem* rs) = 0;
  VertexDesc* vertex_desc() { return desc_;}
 protected:
  VertexDescPtr desc_;
  ShaderInfo info_;
  std::string error_msg_;
  DISALLOW_COPY_AND_ASSIGN(Shader);
};


CharType* ShaderSuffix(ShaderType type);
AZER_EXPORT bool LoadShader(int stage, const std::string& path, ShaderInfo* shader);
AZER_EXPORT bool LoadShaderOnFS(int stage, const ResPath& path, 
                                ShaderInfo* info, FileSystem* fs);
AZER_EXPORT bool LoadShader(int stage, const std::string& path, TechSource*);
AZER_EXPORT bool LoadShaderOnFS(int stage, const ResPath& path, TechSource* info, 
                                FileSystem* fs);
}  // namespace azer
