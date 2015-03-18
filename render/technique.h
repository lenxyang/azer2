#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "azer/base/export.h"
#include "azer/render/gpu_program.h"

namespace azer {

class Renderer;

class VertexDesc;
class GpuProgram;
class VertexGpuProgram;
typedef scoped_refptr<GpuProgram> GpuProgramPtr;
typedef scoped_refptr<VertexGpuProgram> VertexGpuProgramPtr;
typedef scoped_refptr<VertexDesc> VertexDescPtr;

class AZER_EXPORT Technique : public ::base::RefCounted<Technique> {
 public:
  virtual ~Technique();
  void AddGpuProgram(GpuProgramPtr& gpu);
  void AddGpuProgram(VertexGpuProgramPtr& gpu);

  virtual void Use(Renderer*) = 0;
  VertexDescPtr GetVertexDesc();
 protected:
  Technique();

  std::vector<GpuProgramPtr> pline_;
  DISALLOW_COPY_AND_ASSIGN(Technique);
};

typedef scoped_refptr<Technique> TechniquePtr;

}  // namespace azer
