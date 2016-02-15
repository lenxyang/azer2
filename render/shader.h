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
typedef scoped_refptr<VertexDesc> VertexDescPtr;

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
  virtual ~Shader();

  std::string error_msg() const { return error_msg_;}
  RenderPipelineStage stage() const { return stage_;}

  virtual bool Init(RenderSystem* rs) = 0;

  /**
   * 每一个 shader 都有对应的输入输出格式，通过以下接口可以检查
   * Render Pipeline 的过程当中各阶段输入输出是否匹配
   */
  virtual VertexDescPtr GetInputDesc() { return NULL;}
 protected:
  Shader(const ShaderInfo& info);

  RenderPipelineStage stage_;
  ShaderInfo info_;
  std::string error_msg_;
  DISALLOW_COPY_AND_ASSIGN(Shader);
};

class AZER_EXPORT VertexShader : public Shader {
 public:
  virtual VertexDescPtr GetInputDesc() { return desc_ptr_;}
 protected:
  VertexShader(VertexDescPtr& desc, const ShaderInfo& info);

  VertexDescPtr desc_ptr_;
  DISALLOW_COPY_AND_ASSIGN(VertexShader);
};

CharType* ShaderSuffix(ShaderType type);


typedef scoped_refptr<Shader> ShaderPtr;
typedef scoped_refptr<VertexShader> VertexShaderPtr;

AZER_EXPORT bool LoadShaderInfo(int stage, const std::string& path, 
                                 ShaderInfo* shader);
AZER_EXPORT bool LoadShaderInfoOnFS(int stage, const ResPath& path, 
                                     ShaderInfo* info, FileSystem* fs);
AZER_EXPORT bool LoadShaderInfo(int stage, const std::string& path, TechSource*);
AZER_EXPORT bool LoadShaderInfoOnFS(int stage, const ResPath& path, 
                                     TechSource* info, FileSystem* fs);
}  // namespace azer
