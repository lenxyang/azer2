#pragma once

#include <string>
#include <map>

#include "base/basictypes.h"
#include "base/synchronization/lock.h"
#include "base/thread/lock.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "azer/base/resource_pack.h"

namespace azer {

class Effect;
class EffectAdapterContext;
class ResourcePack;
class Texture;

class ResLib {
 public:
  static ResLib* instance();

  EffectLib * effectlib();
  Effect* GetEffect*(const std::string& name);
  Texture* GetTexture(int32 id);
  EffectAdapterContext* adapter_context() { return adapter_context_.get();}
  ResourcePack* resource_pack() { return resource_pack_.get();}
 private:
  ResLib();
  ~ResLib();

  ::base::Mutex lock_;
  scoped_ptr<EffectLib> effectlib_;
  std::map<int, scoped_refptr<Texture> > textures_;
  scoped_ptr<ResourcePack> resource_pack_;
  DISALLOW_COPY_AND_ASSIGN(ResLib);
};
}  // namespace azer
