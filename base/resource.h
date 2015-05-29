#pragma once

#include <string>
#include <memory>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/values.h"
#include "azer/base/export.h"

namespace azer {

class RenderSystem;

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
    kScene,
    kVertexDesc,
    kVertexData,
    kIndicesData,
    kMeshObject,
    kConfig,
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
