#pragma once

#include "azer/render/texture.h"
#include "azer/render/resource_view.h"
#include "azer/render/gpu_buffer_view.h"

namespace azer {
class TextureView;
class UATextureView;
typedef scoped_refptr<TextureView> TextureViewPtr;
typedef scoped_refptr<UATextureView> UATextureViewPtr;

class AZER_EXPORT TextureView : public ShaderResView {
 public:
  struct AZER_EXPORT Options {
    TexFormat format;            // default: kRGBAn8
    TexType type;

    Options();
  };

  TextureView(const Options& options, Texture* tex);
  const Options& options() const { return options_;}
  Texture* texture() { return tex_.get();}
  virtual void GenerateMips(int32_t level) = 0;
 private:
  TexturePtr tex_;
  const Options options_;
  DISALLOW_COPY_AND_ASSIGN(TextureView);
};

class AZER_EXPORT UATextureView : public UnorderAccessResView {
 public:
  struct AZER_EXPORT Options {
    TexFormat format;            // default: kRGBAn8
    TexType type;

    Options();
  };

  UATextureView(const Options& options, Texture* tex);
  const Options& options() const { return options_;}
  Texture* texture() { return tex_.get();}
  virtual void GenerateMips(int32_t level) = 0;
 private:
  TexturePtr tex_;
  const Options options_;
  DISALLOW_COPY_AND_ASSIGN(UATextureView);
};
}  // namespace azer
