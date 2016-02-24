#include "azer/effect/effectlib.h"

#include "base/logging.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/effect/effect.h"
#include "azer/effect/effect_creator.h"
#include "azer/effect/color_effect.h"
#include "azer/effect/diffusemap_effect.h"
#include "azer/effect/texture_effect.h"
#include "azer/effect/diffusemap_effect_adapter.h"
#include "azer/effect/normalline_effect.h"
#include "azer/effect/shaderlib/grit/shaderlib.hlsl.h"
#include "azer/render/blending.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/technique.h"

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
  {"ColorEffect", ColorEffect::kEffectName, 0, 
   HLSL_DIFFUSEMAP_VS, 0, 0, 0, HLSL_DIFFUSEMAP_PS},
  {"ColorEffect", ColorEffect::kEffectName, 0, 
   HLSL_DIFFUSEMAP_VS, 0, 0, 0, HLSL_DIFFUSEMAP_PS},
  {"NormalLineEffect", NormalLineEffect::kEffectName, 0, 
   HLSL_NORMALLINE_VS, 0, 0, HLSL_NORMALLINE_GS, HLSL_NORMALLINE_PS},
  {"TextureEffect0", TextureEffect::kEffectName, 1, 
   HLSL_TEXTURE0_VS, 0, 0, 0, HLSL_TEXTURE0_PS},
};
}  // namespace effect

EffectLib::EffectLib() {}
bool EffectLib::Load(const base::FilePath& filepath) {
  InitAdapterContext();
  resource_bundle_.reset(new effectlib::ResourceBundle);
  return resource_bundle_->Load(filepath);
}

Effect* EffectLib::GetEffect(const std::string& name) {
  auto iter = effects_.find(name);
  if (iter != effects_.end()) {
    return iter->second.get();
  } else {
    return LoadEffect(name);
  }
}

namespace {
void LoadEffectData(EffectData* data, TechSource* source,
                    effectlib::ResourceBundle* res) {
  if (data->vertex_shader_id > 0) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".vs.hlsl";
    shader.stage = kVertexStage;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(
        data->vertex_shader_id, ui::ScaleFactor(1));
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }

  if (data->hull_shader_id > 0) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".ds.hlsl";
    shader.stage = kHullStage;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(
        data->hull_shader_id, ui::ScaleFactor(1));
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }

  if (data->domain_shader_id > 0) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".ds.hlsl";
    shader.stage = kDomainStage;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(
        data->domain_shader_id, ui::ScaleFactor(1));
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }

  if (data->geometry_shader_id > 0) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".ds.hlsl";
    shader.stage = kGeometryStage;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(
        data->geometry_shader_id, ui::ScaleFactor(1));
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }

  if (data->pixel_shader_id > 0) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".ds.hlsl";
    shader.stage = kPixelStage;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(
        data->pixel_shader_id, ui::ScaleFactor(1));
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
      LoadEffectData(data, &tech, resource_bundle_.get());
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

bool LoadEffectLib(EffectLib* lib) {
  using base::UTF8ToUTF16;
  base::FilePath path(UTF8ToUTF16("out/dbg/shaderlib.hlsl.pak"));
  return lib->Load(path);
}
}  // namespace azer
