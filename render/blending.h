#pragma once

#include <string>
#include <memory>


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

  struct AZER_EXPORT RenderTargetDesc {
    Blend src;
    Blend dest;
    Oper oper;
    Blend src_alpha;
    Blend dest_alpha;
    Oper alpha_oper;
    WriteMask mask;
    bool enable;
    RenderTargetDesc();
  };

  static const int32_t kMaxRenderTargetDesc = 16;
  struct AZER_EXPORT BlendDesc {
    RenderTargetDesc desc[kMaxRenderTargetDesc];
    bool alpha_to_converage;
    bool independent_blending;

    BlendDesc();
  };
 public:
  explicit Blending(const BlendDesc& desc);
  virtual ~Blending();
  const BlendDesc& desc() const { return desc_;}
 protected:
  virtual bool OnDescChanged() = 0;

  BlendDesc desc_;
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
