#include "azer/render/reusable_object.h"

#include "azer/render/render_system.h"

namespace azer {

namespace {
BlendingPtr CreateDefaultBlending(RenderSystem* rs) {
  Blending::Desc blend_desc;
  blend_desc.src = Blending::kSrcAlpha;
  blend_desc.dest = Blending::kSrcInvAlpha;
  blend_desc.oper = Blending::kAdd;
  blend_desc.src_alpha = Blending::kOne;
  blend_desc.dest_alpha = Blending::kZero;
  blend_desc.alpha_oper = Blending::kAdd;
  blend_desc.mask = Blending::kWriteColor;
  return BlendingPtr(rs->CreateBlending(blend_desc));
}
}  // namespace

ReusableObject::ReusableObject() {
}

ReusableObject::~ReusableObject() {
}

bool ReusableObject::Init(RenderSystem* rs) {
  BlendingPtr ptr = CreateDefaultBlending(rs);
  if (!ptr.get()) {
    LOG(ERROR) << "Failed to create blending.";
    return false;
  }

  CHECK(InsertBlending("default", ptr));
  return true;
}

bool ReusableObject::InsertBlending(const std::string& name, BlendingPtr& ptr) {
  auto iter = blending_dict_.find(name);
  if (iter == blending_dict_.end() || iter->second.get() == NULL) {
    blending_dict_[name] = ptr;
    return true;
  } else {
    return false;
  }
}

bool ReusableObject::InsertVertexDesc(const std::string& name, 
                                       VertexDescPtr& ptr) {
  auto iter = vertex_desc_dict_.find(name);
  if (iter == vertex_desc_dict_.end() || iter->second.get() == NULL) {
    vertex_desc_dict_[name] = ptr;
    return true;
  } else {
    return false;
  }
}

bool ReusableObject::InsertGpuProgram(const std::string& name, GpuProgramPtr& ptr) {
  auto iter = program_dict_.find(name);
  if (iter == program_dict_.end() || iter->second.get() == NULL) {
    program_dict_[name] = ptr;
    return true;
  } else {
    return false;
  }
}

BlendingPtr& ReusableObject::GetBlending(const std::string& name) {
  return blending_dict_[name];
}

VertexDescPtr& ReusableObject::GetVertexDesc(const std::string& name) {
  return vertex_desc_dict_[name];
}

GpuProgramPtr& ReusableObject::GetGpuProgram(const std::string& name) {
  return program_dict_[name];
}
}  // namespace azer
