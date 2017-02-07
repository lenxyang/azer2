#include "azer/res/reslib.h"

#include "base/files/file_path.h"
#include "azer/base/resource_pack.h"
#include "azer/render/effect.h"
#include "azer/effect/effectlib.h"
#include "azer/effect/effect_params_adapter.h"
#include "azer/util/images/image.h"
#include "azer/render/render_system.h"

namespace azer {
namespace {
::base::LazyInstance<ResLib> env_instance = LAZY_INSTANCE_INITIALIZER;
}

ResLib* ResLib::instance() {
  return env_instance.Pointer();
}

ResLib::ResLib() {
  ::base::FilePath effectpath(FILE_PATH_LITERAL("out/dbg/azer.pak"));
  resource_pack_.reset(new ResourcePack);
  CHECK(resource_pack_->Load(effectpath));
  effectlib_.reset(new EffectLib(resource_pack_.get()));
}

ResLib::~ResLib() {}

EffectLib * ResLib::effectlib() { return effectlib_.get();}
Effect* ResLib::GetEffect(const std::string& name) {
  return effectlib_->GetEffect(name);
}

Texture* ResLib::GetTexture(int32_t id) {
  ::base::AutoLock scoped_lock(lock_);
  auto iter = texture_.find(id);
  if (texture_.end() != iter) {
    return iter->second.get();
  }

  RenderSystem* rs = RenderSystem::Current();
  base::RefCountedStaticMemory* memory = resource_pack_->LoadDataResourceBytes(id);
  CHECK(memory);
  ImageDataPtr img = LoadDDSImageFromMemory(
      memory->front(), static_cast<int32_t>(memory->size()));
  Texture::Options opt;
  opt.target = kBindTargetShaderResource;
  TexturePtr tex = rs->CreateTexture2D(opt, img.get());
  texture_.insert(std::make_pair(id, tex));
  return tex.get();
}

EffectAdapterContext* ResLib::effect_context() { 
  return effectlib_->adapter_context();
}

ResourcePack* ResLib::resource_pack() { 
  return resource_pack_.get();
}

}  // namespace azer
