#pragma once

#include <memory>
#include <string>
#include <vector>


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

/**
 * options:
 *   "debug"
 *   "optimization"
 *   "strictness"
 *   "prefer_flow_control"
 *
 **/
struct AZER_EXPORT ShaderInfo {
  ShaderInfo();
  int32_t stage;
  int32_t format;
  std::string code;
  std::string name;
  std::string path;
  std::string version;
  std::string entry;
  std::string defines;
  std::string includes;
  std::string options;
};

class AZER_EXPORT Shader : public ::base::RefCounted<Shader> {
 public:
  virtual ~Shader();

  std::string error_msg() const { return error_msg_;}
  RenderPipelineStage stage() const;

  VertexDesc* vertex_desc() { return desc_.get();}
 protected:
  explicit Shader(const ShaderInfo& info);
  Shader(VertexDesc* desc, const ShaderInfo& info);

  VertexDescPtr desc_;
  ShaderInfo info_;
  std::string error_msg_;
  DISALLOW_COPY_AND_ASSIGN(Shader);
};


AZER_EXPORT CharType* ShaderSuffix(ShaderType type);
AZER_EXPORT bool LoadShader(int stage, const std::string& path, ShaderInfo* shader);
AZER_EXPORT bool LoadShader(int stage, const std::string& path, TechSource* tech);
}  // namespace azer
