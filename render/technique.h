#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/base/export.h"
#include "azer/render/shader.h"

namespace azer {

class Renderer;

class VertexDesc;
class GpuProgram;
class VertexGpuProgram;
typedef scoped_refptr<GpuProgram> GpuProgramPtr;
typedef scoped_refptr<VertexGpuProgram> VertexGpuProgramPtr;
typedef scoped_refptr<VertexDesc> VertexDescPtr;

class AZER_EXPORT TechSource {
 public:
  struct AZER_EXPORT Options {
    bool use_streamout;
    std::string version;
    Options();
  };

  explicit TechSource(VertexDesc* vertex_desc);
  TechSource(VertexDesc* desc, const Options& options);
  TechSource(VertexDesc* vertex_desc, VertexDesc* streamout_desc, 
          const Options& options);

  const Options& options() const { return options_;}
  const ShaderInfo& operator[](const int32 index) const;
  int32 size() const { return kRenderPipelineStageNum;}
  VertexDesc* vertex_desc() const { return vertex_desc_;}
  VertexDesc* streamout_vertex_desc() const { return streamout_desc_;}
  void SetStage(int32 stage, const ShaderInfo& shader);
  void AddShader(const ShaderInfo& shader);
 private:
  VertexDescPtr vertex_desc_;
  VertexDescPtr streamout_desc_;
  ShaderInfo shaders_[kRenderPipelineStageNum];
  const Options options_;
  DISALLOW_COPY_AND_ASSIGN(TechSource);
};

class AZER_EXPORT Technique : public ::base::RefCounted<Technique> {
 public:
  virtual ~Technique();
  void AddShader(ShaderPtr& gpu);
  void AddShader(VertexShaderPtr& gpu);

  virtual void Use(Renderer*) = 0;
  VertexDesc* GetVertexDesc();
 protected:
  Technique();

  std::vector<ShaderPtr> pline_;
  DISALLOW_COPY_AND_ASSIGN(Technique);
};

typedef scoped_refptr<Technique> TechniquePtr;

}  // namespace azer
