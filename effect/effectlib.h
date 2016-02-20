#pragma once

#include <string>
#include <map>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "azer/effect/resource_bundle.h"

namespace azer {

class Effect;

class EffectLib {
 public:
  EffectLib();
  bool Load(const base::FilePath& filepath);
  Effect* GetEffect(const std::string& name);
 private:
  Effect* LoadEffect(const std::string& name);
  std::map<std::string, scoped_refptr<Effect> > effects_;
  scoped_ptr<effectlib::ResourceBundle> resource_bundle_;
  DISALLOW_COPY_AND_ASSIGN(EffectLib);
};
}  // namespace azer
