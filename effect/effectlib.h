#pragma once

#include <string>
#include <map>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "azer/base/resource_bundle.h"

namespace azer {

class Effect;

class EffectLib {
 public:
  EffectLib();
  bool Load(const base::FilePath& filepath);
  Effect* GetEffect(int32 id);
 private:
  Effect* LoadEffect(int32 id);
  std::map<int32, scoped_refptr<Effect> > effects_;
  DISALLOW_COPY_AND_ASSIGN(EffectLib);
};
}  // namespace azer
