#include "azer/render/reusable_object_util.h"

#include "base/logging.h"
#include "azer/render/render_system.h"
#include "azer/render/gpu_program.h"

namespace azer {
GpuProgramPtr GetVertexProgramMayCreate(const std::string& name,
                                        const std::string& program,
                                        VertexDescPtr& vdesc) {
  RenderSystem* rs = RenderSystem::Current();
  ReusableObject* objs = rs->GetReusableObject();
  GpuProgramPtr vs(objs->GetGpuProgram(name));
  if (!vs.get()) {
    vs = rs->CreateVertexGpuProgram(vdesc, program.c_str());
    if (vs.get() == NULL) {
      LOG(ERROR) << "Overlay VertexStage Program compiled failed";  
      return GpuProgramPtr();
    }
    DCHECK(objs->InsertGpuProgram(name, vs));
  }

  return vs;
}

GpuProgramPtr GetPixelProgramMayCreate(const std::string& name,
                                       const std::string& program) {
  RenderSystem* rs = RenderSystem::Current();
  ReusableObject* objs = rs->GetReusableObject();
  GpuProgramPtr ps(objs->GetGpuProgram(name));
  if (!ps.get()) {
    ps = rs->CreateGpuProgram(kPixelStage, program.c_str());
    if (ps.get() == NULL) {
      LOG(ERROR) << "Overlay PiexlStage Program compiled failed";  
      return GpuProgramPtr();
    }

    DCHECK(objs->InsertGpuProgram(name, ps));
  }

  return ps;
}
}  // namespace azer
