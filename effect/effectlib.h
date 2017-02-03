#pragma once

#include <string>
#include <map>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "azer/base/resource_pack.h"
#include "azer/render/texture.h"

namespace azer {

class Effect;
class EffectAdapterContext;

class EffectLib {
 public:
  EffectLib(ResourcePack* pack);

  static EffectLib* instance();
  Effect* GetEffect(const std::string& name);
  scoped_refptr<Effect> LoadEffect(const std::string& name);
  SamplerStatePtr default_sampler_state();
  EffectAdapterContext* adapter_context() { return adapter_context_.get();}
 private:
  bool InitAdapterContext();

  SamplerStatePtr sampler_state_;
  std::map<std::string, scoped_refptr<Effect> > effects_;
  ResourcePack* resource_pack_;
  std::unique_ptr<EffectAdapterContext> adapter_context_;
  DISALLOW_COPY_AND_ASSIGN(EffectLib);
};

bool LoadEffectLib(EffectLib* lib);
}  // namespace azer
