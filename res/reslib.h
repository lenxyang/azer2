#pragma once

#include <string>
#include <map>
#include <memory>

#include "base/synchronization/lock.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/lazy_instance.h"

namespace azer {

class EffectAdapterContext;
class Effect;
class EffectLib;
class ResourcePack;
class Texture;

class ResLib {
 public:
  static ResLib* instance();

  EffectLib * effectlib();
  Effect* GetEffect(const std::string& name);
  Texture* GetTexture(int32_t id);
  EffectAdapterContext* effect_context();
  ResourcePack* resource_pack();
 private:
  ResLib();
  ~ResLib();

  ::base::Lock lock_;
  ::std::unique_ptr<EffectLib> effectlib_;
  std::map<int, scoped_refptr<Texture> > texture_;
  std::unique_ptr<ResourcePack> resource_pack_;
  friend struct ::base::DefaultLazyInstanceTraits<ResLib>;
  DISALLOW_COPY_AND_ASSIGN(ResLib);
};
}  // namespace azer
