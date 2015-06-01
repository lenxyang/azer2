/**
 * light 和 material 的结构可能非常复杂，因此框架并给出明确的结构定义
 * 而仅仅使用 id 进行标识。在实际使用过程中（即游戏引擎当中），会预先定义一组
 * Light，Material，及 Effect。Effect 会提供函数 SetLight 和 SetMatrial 将
 * Light 和 Matrial 传递到 shader 当中。这几个函数的具体实现由最终用户完成。
 *
 */
#pragma once

#include <string>
#include "base/memory/ref_counted.h"

namespace azer {
class Light : public ::base::RefCounted<Light> {
 public:
  explicit Light(int32 id);
  virtual ~Light() {}

  int32 id() const { return id_;}
 private:
  int32 id_;
  DISALLOW_COPY_AND_ASSIGN(Light);
};

typedef scoped_refptr<Light> LightPtr;
}  // namespace azer
