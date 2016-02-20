#include "azer/effect/effectlib.h"

#include "base/logging.h"
#include "base/files/file_util.h"
#include "azer/effect/effect.h"
#include "azer/effect/shaderlib/grit/shaderlib.hlsl.h"

namespace azer {
EffectLib::EffectLib() {}

bool EffectLib::Load(const base::FilePath& filepath) {
  std::string contents;
  if (!::base::ReadFileToString(filepath, &contents)) {
    PLOG(ERROR) << "Failed to readfile \"" << filepath.value() << "\"";
    return false;
  }

  return true;
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
