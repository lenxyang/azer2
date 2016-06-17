#include "azer/effect/effectlib.h"

#include "base/logging.h"
#include "base/files/file_path.h"
#include "base/files/file_util.h"
#include "base/lazy_instance.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/render/blending.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/technique.h"
#include "azer/effect/effect.h"
#include "azer/effect/effect_creator.h"
#include "azer/effect/color_effect.h"
#include "azer/effect/diffusemap_effect.h"
#include "azer/effect/diffusemap_effect_adapter.h"
#include "azer/effect/normalline_effect.h"
#include "azer/effect/overlay_effect.h"
#include "azer/effect/sky_effect.h"
#include "azer/effect/texture_effect.h"
#include "azer/effect/text_billboard_effect.h"
#include "azer/res/grit/hlsllib.h"

namespace azer {
namespace {
const VertexDesc::Desc kVertexDesc[][10] = {
  {
    {"POSITION", 0, kVec3},
    {NULL, 0, kVec3}
  },
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
  int32_t vertex_layout_index;
  int32_t shaderid;
  const char* vsentry;
  const char* hsentry;
  const char* dsentry;
  const char* gsentry;
  const char* psentry;
};

EffectData effect_data[] = {
  {"TextBillboardEffect", TextBillboardEffect::kEffectName, 0, 
   HLSLLIB_TEXTBILLBOARD, "vs_main", 0, 0, "gs_main", "ps_main"},
  {"AmbientColorEffect", AmbientColorEffect::kEffectName, 1, 
   HLSLLIB_AMBIENT, "vs_main", 0, 0, 0, "ps_main"},
  {"ColorEffect", ColorEffect::kEffectName, 1, HLSLLIB_COLOR, 
   "vs_main", 0, 0, 0, "ps_main"},
  {"DiffuseMapEffect", DiffuseMapEffect::kEffectName, 1, 
   HLSLLIB_DIFFUSEMAP, "vs_main", 0, 0, 0, "ps_main"},
  {"InstDiffuseMapEffect", DiffuseMapEffect::kEffectName, 1, 
   HLSLLIB_INST_DIFFUSEMAP, "vs_main", 0, 0, 0, "ps_main"},
  {"NormalLineEffect", NormalLineEffect::kEffectName, 1, 
   HLSLLIB_NORMALLINE, "vs_main", 0, 0, "gs_main", "ps_main"},
  {"TextureEffect0", TextureEffect::kEffectName, 2, 
   HLSLLIB_TEXTURE, "vs_main0", 0, 0, 0, "ps_main0"},
  {"TextureEffect1", TextureEffect::kEffectName, 2, 
   HLSLLIB_TEXTURE, "vs_main1", 0, 0, 0, "ps_main1"},
  {"SkyboxEffect", SkyboxEffect::kEffectName, 2, 
   HLSLLIB_SKYBOX, "vs_main", 0, 0, 0, "ps_main"},
  {"OverlayEffect", OverlayEffect::kEffectName, 2,
   HLSLLIB_OVERLAY, "vs_main", 0, 0, 0, "ps_main"},
  {"MSOverlayEffect", OverlayEffect::kEffectName, 2,
   HLSLLIB_OVERLAY, "vs_main", 0, 0, 0, "ps_main_ms"},
};

class EffectLibHolder {
 public:
  EffectLibHolder() {
    ::base::FilePath effectpath(FILE_PATH_LITERAL("out/dbg/azer.pak"));
    resource_pack_.reset(new ResourcePack);
    CHECK(resource_pack_->Load(effectpath));
    effectlib_.reset(new EffectLib(resource_pack_.get()));
  }
  EffectLib* GetEffectLib() { return effectlib_.get();}
 private:
  std::unique_ptr<ResourcePack> resource_pack_;
  std::unique_ptr<EffectLib> effectlib_;
  DISALLOW_COPY_AND_ASSIGN(EffectLibHolder);
};
::base::LazyInstance<EffectLibHolder> env_instance = LAZY_INSTANCE_INITIALIZER;
}  // namespace effect


EffectLib* EffectLib::instance() {
  return env_instance.Pointer()->GetEffectLib();
}

EffectLib::EffectLib(ResourcePack* pack) : resource_pack_(pack) {
  InitAdapterContext();
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
void LoadEffectData(EffectData* data, TechSource* source, ResourcePack* res) {
  if (data->vsentry) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".vs.hlsl";
    shader.stage = kVertexStage;
    shader.entry = data->vsentry;
    int32_t id = data->shaderid;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(id);
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }

  if (data->hsentry) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".hs.hlsl";
    shader.stage = kHullStage;
    shader.entry = data->hsentry;
    int32_t id = data->shaderid;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(id);
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }

  if (data->dsentry) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".ds.hlsl";
    shader.stage = kDomainStage;
    shader.entry = data->dsentry;
    int32_t id = data->shaderid;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(id);
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }

  if (data->gsentry) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".gs.hlsl";
    shader.stage = kGeometryStage;
    shader.entry = data->gsentry;
    int32_t id = data->shaderid;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(id);
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }

  if (data->psentry) {
    ShaderInfo shader;
    shader.path = std::string(data->name) + ".ps.hlsl";
    shader.stage = kPixelStage;
    shader.entry = data->psentry;
    int32_t id = data->shaderid;
    base::RefCountedStaticMemory* memory = res->LoadDataResourceBytes(id);
    shader.code = std::string((const char*)memory->front(), memory->size());
    source->AddShader(shader);
  }
}
}

Effect* EffectLib::LoadEffect(const std::string& name) {
  for (uint32_t i = 0; i < arraysize(effect_data); ++i) {
    if (std::string(effect_data[i].name) == name) {
      EffectData* data = effect_data + i;
      EffectPtr effect(CreateEffectByName(data->effect_name));
      int32_t vdindex = data->vertex_layout_index;
      VertexDescPtr desc(new VertexDesc(kVertexDesc[vdindex]));
      TechSource tech(desc.get());
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
