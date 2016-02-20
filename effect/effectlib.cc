#include "azer/effect/effectlib.h"

#include "base/logging.h"
#include "base/files/file_util.h"
#include "azer/effect/vertex_buffer.h"
#include "azer/effect/effect.h"
#include "azer/effect/shaderlib/grit/shaderlib.hlsl.h"

namespace azer {
namespace {
const VertexDesc::Desc kVertexDesc[][] = {{
    {"POSITION", 0, kVec3},
    {"NORMAL", 0, kVec3},
    {"TEXCOORD", 0, kVec2},
  };
};

struct EffectData {
  const char* effect_name;
  int32 vertex_desc_index;
  int32 vertex_shader_id;
  int32 hull_shader_id;
  int32 domain_shader_id;
  int32 geometry_shader_id;
  int32 pixel_shader_id;
};

EffectData effect_data[] = {
  {"Effect", 0, 100, 0, 0, 0, 100},
};
}  // namespace effect

EffectLib::EffectLib() {}
bool EffectLib::Load(const base::FilePath& filepath) {
  resource_bundle_.reset(new ResourceBundle);
  return resource_bundle_->Load(filepath);
}

Effect* EffectLib::GetEffect(const int32 id) {
  auto iter = effects_.find(id);
  if (iter != effects_.end()) {
    return iter->second.get();
  } else {
    return LoadEffect(id);
  }
}

Effect* EffectLib::LoadEffect(int32 id) {
  return NULL;
}
}  // namespace azer
