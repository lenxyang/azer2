#pragma once

#include <string>
#include "base/values.h"
#include "azer/math/math.h"

namespace azer {
namespace resources {
bool GetFloat(const base::DictionaryValue* dict, const std::string& path,
              float* v, float default_value = 0.0f);

bool GetInt(const base::DictionaryValue* dict, const std::string& path,
            int* v, int default_value = 0);

bool GetVector3(const base::DictionaryValue* dict, const std::string& path,
                Vector3* vec);

bool GetVector4(const base::DictionaryValue* dict, const std::string& path,
                Vector4* vec);
bool GetQuaternion(const base::DictionaryValue* dict,
                   const std::string& path, Quaternion* quaternion);
}  // namespace resources
}  // namespace azer
