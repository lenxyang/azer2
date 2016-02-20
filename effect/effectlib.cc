#include "azer/effect/effectlib.h"

#include "base/logging.h"
#include "base/files/file_util.h"
#include "azer/effect/effect.h"
#include "azer/effect/effect_creator.h"
#include "azer/effect/shaderlib/grit/shaderlib.hlsl.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/technique.h"

namespace azer {
namespace {
const VertexDesc::Desc kVertexDesc[1][10] = {
  {
    {"POSITION", 0, kVec3},
    {"NORMAL", 0, kVec3},
    {"TEXCOORD", 0, kVec2},
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
  {"DiffuseMapEffect", "DiffuseMapEffect", 0, 100, 0, 0, 0, 100},
};
}  // namespace effect

EffectLib::EffectLib() {}
bool EffectLib::Load(const base::FilePath& filepath) {
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

Effect* EffectLib::LoadEffect(const std::string& name) {
  for (uint32 i = 0; i < arraysize(effect_data); ++i) {
    if (std::string(effect_data[i].name) == name) {
      EffectData* data = effect_data + i;
      EffectPtr effect(CreateEffectByName(data->effect_name));
      int32 vdindex = data->vertex_desc_index;
      VertexDescPtr desc(
          new VertexDesc(kVertexDesc[vdindex], arraysize(kVertexDesc[vdindex])));
      TechSource tech(desc);
      CHECK(effect->Init(tech)) << "Effect \"" << name << "\" init failed";
      effects_.insert(std::make_pair(name, effect));
      return effect.get();
    }
  }

  LOG(ERROR) << "No such effect \"" << name << "\" in effectlib";
  return NULL;
}
}  // namespace azer
