#pragma once

#include "base/basictypes.h"
#include "base/values.h"
#include "azer/base/export.h"

namespace azer {

class Resource;

namespace resource {

/**
 * 针对每一种资源，都需要一类 ResourceLoader 与之对应
 * ResourceLoader 完成的功能是根据配置文件加载对应的 Resource 创建，
 * 初始化等功能
 * ResourceLoader 的创建有 ResourceLoaderCreatro 完成
 */
class AZER_EXPORT ResourceLoader {
 public:
  ResourceLoader(ResourceLoaderCreator* creator) : creator_(creator) {}
  virtual ~ResourceLoader() {}
  virtual Resource* Load(const ::base::Value& value) = 0;

  const std::string& name() const { return name_;}
 protected:
  std::string name_;
  ResourceLoaderCreator* creator_;
  DISALLOW_COPY_AND_ASSIGN(ResourceLoader);
};

typedef std::shared_ptr<ResourceLoader> ResourceLoaderPtr;

class AZER_EXPORT ResourceLoaderCreator {
 public:
  ResourceLoaderCreator(RenderSystem* rs) : render_system_(rs) {}
  virtual ~ResourceLoaderCreator() {}
  
  virtual ResourceLoaderPtr GetLoader(const std::string& loader_name) = 0;
 private:
  RenderSystem* render_system_;
  DISALLOW_COPY_AND_ASSIGN(ResourceLoaderCreator);
};

}  // namespace resource
}  // namespace azer
