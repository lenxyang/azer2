#pragma once

#include <string>
#include <memory>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/logging.h"
#include "azer/base/export.h"

namespace azer {
class Renderer;

class AZER_EXPORT Blending : public ::base::RefCounted<Blending> {
 public:
  enum Oper {
    kAdd = 1,
    kSub,
    kRevsub,
    kMin,
    kMax,
  };
  enum Blend {
    kZero = 0,
    kOne,
    kSrcColor,
    kSrcInvColor,
    kSrcAlpha,
    kSrcInvAlpha,
    kSrcAlphaSat,
    kSrc1Color,
    kSrc1InvColor,
    kSrc1Alpha,
    kSrc1InvAlpha,
    
    kDestColor,
    kDestInvColor,
    kDestAlpha,
    kDestInvAlpha,

    kFactor,
    kInvFactor,
  };

  enum WriteMask {
    kWriteR = 0x0001L,
    kWriteG = 0x0002L,
    kWriteB = 0x0004L,
    kWriteA = 0x0008L,
    kWriteColor = (kWriteR | kWriteG | kWriteB),
    kWriteAll = (kWriteColor | kWriteA),
  };

  struct Desc {
    Blend src;
    Blend dest;
    Oper oper;
    Blend src_alpha;
    Blend dest_alpha;
    Oper alpha_oper;
    WriteMask mask;
    Desc()
        : src(kOne)
        , dest(kZero)
        , oper(kAdd)
        , src_alpha(kOne)
        , dest_alpha(kZero)
        , alpha_oper(kAdd)
        , mask(kWriteAll) {
    }
  };

 public:
  explicit Blending(const Desc& desc);
  virtual ~Blending();
  const Desc& desc() const { return desc_;}
  void EnableAlphaToConverage(bool enable);
  void EnableIndependentBlend(bool enable);
 protected:
  virtual bool OnDescChanged() = 0;

  bool enable_alpha_to_converage_;
  bool enable_independent_blending_;
  Desc desc_;
  DISALLOW_COPY_AND_ASSIGN(Blending);
};

typedef scoped_refptr<Blending> BlendingPtr;

class AZER_EXPORT ScopedResetBlending {
 public:
  ScopedResetBlending(Renderer* renderer);
  ~ScopedResetBlending();
 private:
  Renderer* renderer_;
  DISALLOW_COPY_AND_ASSIGN(ScopedResetBlending);
};
}  // namespace azer
