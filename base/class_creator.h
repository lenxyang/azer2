#pragma once

#include <map>
#include <string>
#include "base/callback.h"

template<class T>
class ClassCreator {
 public:
  typedef ::base::Callback<T*()> CreatorFunc;
  typedef std::map<std::string, CreatorFunc> Creators;

  void Register(const std::string& classname, const CreatorFunc& creator) {
    creators_.insert(std::make_pair(classname, creator));
  }
  static T* create(const std::string& classname) {
    return instance()->create_object(classname);
  }

  static ClassCreator<T>* instance() {
    static ClassCreator<T> creator;
    return &creator;
  }
 private:
  T* create_object(const std::string& classname) {
    auto iter = creators_.find(classname);
    if (iter == creators_.end())
      return NULL;

    return iter->second->Run();
  }
  Creators creators_;
  DISALLOW_COPY_AND_ASSIGN(ClassCreator);
};

#define OBJECT_ENTRY_AUTO(CLASSNAME, NAME, FUNC)                \
  ClassCreator<CLASSNAME>::instance()->Register(NAME, &FUNC);
  
