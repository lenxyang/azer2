#include "azer/effect/effectlib.h"

#include "base/logging.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/render/blending.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/technique.h"
#include "azer/effect/effect.h"
#include "azer/effect/effect_creator.h"
#include "azer/effect/color_effect.h"
#include "azer/effect/diffusemap_effect.h"
#include "azer/effect/diffusemap_effect_adapter.h"
#include "azer/effect/texture_effect.h"
#include "azer/effect/text_billboard_effect.h"
#include "azer/effect/sky_effect.h"
#include "azer/effect/normalline_effect.h"
#include "azer/res/grit/hlsllib.h"

namespace azer {
namespace {
const VertexDesc::Desc kVertexDesc[][10] = {
  {
    {"POSITION", 0, kVec3},
    {"NORMAL", 0, kVec3},
    {"TEXCOORD", 0, kVec2},
    {NULL, 0, kVec3}
  },
  {
    {"POSITION", 0, kVec3},
    {"NORMAL", 0, kVec3},
    {"TEXCOORD", 0, kVec2},
    {"TANGENT", 0, kVec3},
    {NULL, 0, kVec3}
  },
};

struct EffectData {
  const char* name;
  const char* effect_name;
  int32 vertex_desc_index;
  int32 vertex_shader_id;
  int32 hull_shader_id;
  int32 domain_shader_id;
  int32 geometry_shader_id;
  int32 pixel_shader_id;
};

EffectData effect_data[] = {
  {"AmbientColorEffect", AmbientColorEffect::kEffectName, 0, 
   HLSLLIB_AMBIENT_VS, 0, 0, 0, HLSLLIB_AMBIENT_PS},
  {"TextBillboardEffect", TextBillboardEffect::kEffectName, 0, 
   HLSLLIB_TEXTBILLBOARD_VS, 0, 0, HLSLLIB_TEXTBILLBOARD_GS,
   HLSLLIB_TEXTBILLBOARD_PS},
  {"ColorEffect", ColorEffect::kEffectName, 0, 
   HLSLLIB_COLOR_VS, 0, 0, 0, HLSLLIB_COLOR_PS},
  {"ColorEffect", ColorEffect::kEffectName, 0, 
   HLSLLIB_DIFFUSEMAP_VS, 0, 0, 0, HLSLLIB_DIFFUSEMAP_PS},
  {"NormalLineEffect", NormalLineEffect::kEffectName, 0, 
   HLSLLIB_NORMALLINE_VS, 0, 0, HLSLLIB_NORMALLINE_GS, HLSLLIB_NORMALLINE_PS},
  {"TextureEffect0", TextureEffect::kEffectName, 1, 
   HLSLLIB_TEXTURE0_VS, 0, 0, 0, HLSLLIB_TEXTURE0_PS},
  {"TextureEffect1", TextureEffect::kEffectName, 1, 
   HLSLLIB_TEXTURE1_VS, 0, 0, 0, HLSLLIB_TEXTURE1_PS},
  {"SkyboxEffect", SkyboxEffect::kEffectName, 1, 
   HLSLLIB_SKYBOX_VS, 0, 0, 0, HLSLLIB_SKYBOX_PS},
};
}  // namespace effect

EffectLib::EffectLib(ResourcePack* pack) : resource_pack_(pack) {}

Effect* EffectLib::GetEffect(const std::string& name) {
  auto iter = effects_.find(name);
  if (iter != effects_.end()) {
    return iter->second.get();
  } else {
    return LoadEffect(name);
  }
}

namespace {
void LoadEffectData(EffectData* data, TechSource* source, ResourcePack* res) {
  if (data->vertex_shader_id > 0) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".vs.hlsl";
    shader.stage = kVertexStage;
    int32 id = data->vertex_shader_id;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(id);
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }

  if (data->hull_shader_id > 0) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".hs.hlsl";
    shader.stage = kHullStage;
    int32 id = data->hull_shader_id;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(id);
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }

  if (data->domain_shader_id > 0) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".ds.hlsl";
    shader.stage = kDomainStage;
    int32 id = data->domain_shader_id;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(id);
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }

  if (data->geometry_shader_id > 0) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".gs.hlsl";
    shader.stage = kGeometryStage;
    int32 id = data->geometry_shader_id;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(id);
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }

  if (data->pixel_shader_id > 0) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".ps.hlsl";
    shader.stage = kPixelStage;
    int32 id = data->pixel_shader_id;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(id);
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }
}
}

Effect* EffectLib::LoadEffect(const std::string& name) {
  for (uint32 i = 0; i < arraysize(effect_data); ++i) {
    if (std::string(effect_data[i].name) == name) {
      EffectData* data = effect_data + i;
      EffectPtr effect(CreateEffectByName(data->effect_name));
      int32 vdindex = data->vertex_desc_index;
      VertexDescPtr desc(new VertexDesc(kVertexDesc[vdindex]));
      TechSource tech(desc);
      LoadEffectData(data, &tech, resource_pack_);
      CHECK(effect->Init(tech)) << "Effect \"" << name << "\" init failed";
      effects_.insert(std::make_pair(name, effect));
      return effect.get();
    }
  }

  LOG(ERROR) << "No such effect \"" << name << "\" in effectlib";
  return NULL;
}

bool EffectLib::InitAdapterContext() {
  adapter_context_.reset(new EffectAdapterContext);
  adapter_context_->RegisteAdapter(new DiffuseMapEffect_DiffuseMapMaterial_Adapter);
  adapter_context_->RegisteAdapter(new DiffuseMapEffect_RenderNode_Adapter);
  return true;
}

}  // namespace azer
