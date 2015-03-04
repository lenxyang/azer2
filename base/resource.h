/**
 * = 资源管理 =
 *
 * == 资源的生命周期 ==
 * azer 极端依赖于 shared_ptr，所有的资源都应该以 std::shared_ptr 的形式存在。在没有任何
 * 依赖的情况下， Resource 将被自动释放。但总需要有一个东西来管理他们一边资源不需要重新加载，
 * 即 ResourceManager(azer/resource)。一般说来对资源的最后一个引用都源于这个对象。而
 * ResourceManager 一般是 Scene 级别的，即当场景释放时，隶属于它的 ResourceManager 也将
 * 被释放。
 * 使用 Scene 管理 ResourceManager 有一个极大的优势就是 scene 是层次结构的，因此
 * Resource 的生命周期也能够依赖于这种层次关系而进行优化，例如，某个小场景的资源有可能
 * 来自与他的父节点，所有它的兄弟节点用到类似资源的时候就可以共享，某个小场景释放的时候会
 * 仅释放掉隶属于他的资源
 * 共享资源是一个非常好的方法，但仍然需要帮助，至少在定义场景的时候，需要将那些常用的资源放到
 * 父节点的资源管理模块当中
 */
#pragma once

#include <string>
#include <memory>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/values.h"
#include "azer/base/export.h"

namespace azer {

class RenderSystem;
class ResourceLoaderCreator;

/**
 * all resource can be load from resource manger should 
 * from this class
 */
class AZER_EXPORT Resource : public ::base::RefCounted<Resource> {
 public:
  enum Type {
    kTexture,
    kEffect,
    kScript,
    kVertexDesc,
    kVertexData,
    kIndicesData,
    kMeshObject,
  };

  Resource(Type type) : type_(type) {}
  virtual ~Resource() {}

  Type type() const { return type_;}
 private:
  const Type type_;
  DISALLOW_COPY_AND_ASSIGN(Resource);
};

typedef scoped_refptr<Resource> ResourcePtr;
}  // namespace azer
