#pragma once

#include <map>
#include <string>
#include "base/callback.h"

template<class T>
class ClassCreator {
 public:
  typedef ::base::Callback<T*()> CreatorFunc;
  typedef std::map<std::string, CreatorFunc> Creators;

  ClassCreator() {}

  bool Register(const std::string& classname, const CreatorFunc& creator) {
    auto iter = creators_.find(classname);
    if (iter == creators_.end()) {
      creators_.insert(std::make_pair(classname, creator));
      return true;
    } else {
      return false;
    }
  }
  static T* create(const std::string& classname) {
    return instance()->create_object(classname);
  }

  static ClassCreator<T>* instance() {
    static ClassCreator<T> creator;
    return &creator;
  }

  std::string print_info() const {
    std::string str;
    str.append("ClassCreator\n");
    for (auto iter = creators_.begin(); iter != creators_.end(); ++iter) {
      str.append("  ");
      str.append(iter->first);
      str.append("\n");
    }
    return str;
  }
 private:
  T* create_object(const std::string& classname) {
    auto iter = creators_.find(classname);
    if (iter == creators_.end())
      return NULL;

    return iter->second.Run();
  }
  Creators creators_;
  DISALLOW_COPY_AND_ASSIGN(ClassCreator);
};


#define OBJECT_ENTRY_AUTO(CLASSNAME, NAME, FUNC)                \
  ClassCreator<CLASSNAME>::instance()->Register(NAME, &FUNC);
  
