#pragma once

#include <string>
#include <map>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "azer/base/resource_pack.h"

namespace azer {

class Effect;
class EffectAdapterContext;

class EffectLib {
 public:
  EffectLib(ResourcePack* pack);

  static EffectLib* instance();

  Effect* GetEffect(const std::string& name);
  EffectAdapterContext* adapter_context() { return adapter_context_.get();}
 private:
  Effect* LoadEffect(const std::string& name);
  bool InitAdapterContext();
  std::map<std::string, scoped_refptr<Effect> > effects_;
  ResourcePack* resource_pack_;
  scoped_ptr<EffectAdapterContext> adapter_context_;
  DISALLOW_COPY_AND_ASSIGN(EffectLib);
};

bool LoadEffectLib(EffectLib* lib);
}  // namespace azer
