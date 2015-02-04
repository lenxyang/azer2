#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/lazy_instance.h"

#include "azer/base/export.h"
#include "azer/render/blending.h"
#include "azer/render/gpu_program.h"
#include "azer/render/vertex_data.h"

namespace azer {

class RenderSystem;

// very common used objects
// Notes: please dont't call it every time, just call GetXXX Method
// on initialize and keep it shared_ptr until your wont't used it any more.
class AZER_EXPORT ReusableObject : public ::base::RefCounted<ReusableObject> {
 public:
  ReusableObject();
  virtual ~ReusableObject();

  BlendingPtr& GetBlending(const std::string& name);
  VertexDescPtr& GetVertexDesc(const std::string& name);
  GpuProgramPtr& GetGpuProgram(const std::string& name);

  bool InsertBlending(const std::string& name, BlendingPtr& ptr);
  bool InsertVertexDesc(const std::string& name, VertexDescPtr& ptr);
  bool InsertGpuProgram(const std::string& name, GpuProgramPtr& ptr);

  // why no reusable effect
  // becuase effect take GpuProgram and data, if effected shared,
  // the data will be changed frequenctly, it may result in some problem.
 protected:
  virtual bool Init(RenderSystem* rs);
 private:
  std::map<std::string, BlendingPtr> blending_dict_;
  std::map<std::string, VertexDescPtr> vertex_desc_dict_;
  std::map<std::string, GpuProgramPtr> program_dict_;
  DISALLOW_COPY_AND_ASSIGN(ReusableObject);
};

typedef scoped_refptr<ReusableObject> ReusableObjectPtr;
}  // namespace azer
