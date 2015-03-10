#include "azer/resources/loader/json_util.h"

namespace azer {
namespace resources {
bool GetFloat(const base::DictionaryValue* dict, const std::string& path,
              float* v, float default_value) {
  double db;
  if (dict->GetDouble(path, &db)) {
    *v = (float)db;
    return true;
  } else {
    *v = default_value;
    return false;
  }
}

bool GetInt(const base::DictionaryValue* dict, const std::string& path,
            int* v, int default_value) {
  if (!dict->GetInteger(path, v)) {
    *v = default_value;
    return true;
  } else {
    return false;
  }
}

bool GetVector3(const base::DictionaryValue* dict, const std::string& path,
                Vector3* vec) {
  const ::base::ListValue* list = NULL;
  if (dict->GetList(path, &list)) {
    CHECK_EQ(list->GetSize(), 3u) << path << " should be a float[4]";
    double v1, v2, v3;
    list->GetDouble(0, &v1);
    list->GetDouble(1, &v2);
    list->GetDouble(2, &v3);

    *vec = azer::Vector3((float)v1, (float)v2, (float)v3);
    return true;
  } else {
    return false;
  }
}

bool GetVector4(const base::DictionaryValue* dict, const std::string& path,
                Vector4* vec) {
  const ::base::ListValue* list = NULL;
  if (dict->GetList(path, &list)) {
    CHECK_EQ(list->GetSize(), 4u) << path << " should be a float[4]";
    double v1, v2, v3, v4;
    list->GetDouble(0, &v1);
    list->GetDouble(1, &v2);
    list->GetDouble(2, &v3);
    list->GetDouble(3, &v4);

    *vec = azer::Vector4((float)v1, (float)v2, (float)v3, (float)v4);
    return true;
  } else {
    return false;
  }
}

bool GetQuaternion(const base::DictionaryValue* dict,
                   const std::string& path, Quaternion* quaternion) {
  const ::base::ListValue* list = NULL;
  if (dict->GetList(path, &list)) {
    CHECK_EQ(list->GetSize(), 4u) << path << " should be a float[4]";
    double w, x, y, z;
    list->GetDouble(0, &w);
    list->GetDouble(1, &x);
    list->GetDouble(2, &y);
    list->GetDouble(3, &z);

    *quaternion = Quaternion(w, x, y, z);
    return true;
  } else {
    return false;
  }
}
}  // namespace resources
}  // namespace azer
