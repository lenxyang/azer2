 /**
 * Generated by afx
 * dont edit by hande
 */
#include "azer/samples/triangle/diffuse.afx.h"

#include <stddef.h>

#include "base/basictypes.h"
#include "base/logging.h"

#include "azer/render/render.h"

IMPLEMENT_EFFECT_DYNCREATE(DiffuseEffect);
const char DiffuseEffect::kEffectName[] = "diffuse";
const azer::VertexDesc::Desc DiffuseEffect::kVertexDesc[] = {
  {"POSITION", 0, azer::kVec4, 0, 0, false},
  {"DIFFUSE",  0, azer::kVec4, 0, 0, false},
};

const int DiffuseEffect::kVertexDescNum = arraysize(DiffuseEffect::kVertexDesc);

DiffuseEffect::DiffuseEffect() 
  : azer::Effect(azer::RenderSystem::Current()) {
}

DiffuseEffect::~DiffuseEffect() {
}

const char* DiffuseEffect::name() const {
   return kEffectName;
}
bool DiffuseEffect::Init(const ShaderPrograms& sources) {
  DCHECK(sources.size() == azer::kRenderPipelineStageNum);
  DCHECK(!sources[azer::kVertexStage].code.empty());
  DCHECK(!sources[azer::kPixelStage].code.empty());
  InitTechnique(sources);
  InitGpuConstantTable();
  return true;
}

void DiffuseEffect::InitGpuConstantTable() {
  // generate GpuTable init for stage azer::kVertexStage
  azer::GpuConstantsTable::Desc vs_table_desc[] = {
    azer::GpuConstantsTable::Desc("pvw", azer::GpuConstantsType::kMatrix4,
         offsetof(vs_cbuffer, pvw), 1),
  };
  gpu_table_[azer::kVertexStage] = render_system_->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);
}
void DiffuseEffect::InitTechnique(const ShaderPrograms& sources) {
  vertex_desc_ptr_ = new azer::VertexDesc(kVertexDesc, kVertexDescNum);
  InitShaders(sources);
}

void DiffuseEffect::ApplyGpuConstantTable(azer::Renderer* renderer) {
}

