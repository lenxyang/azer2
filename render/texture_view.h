#pragma once

#include "azer/render/texture.h"
#include "azer/render/resource_view.h"

namespace azer {
class TextureView;
typedef scoped_refptr<TextureView> TextureViewPtr;

class AZER_EXPORT TextureView : public ResourceView {
 public:
  struct AZER_EXPORT Options {
    TexFormat format;            // default: kRGBAn8
    TexType type;

    Options();
  };

  TextureView(const Options& options, Texture* tex);
  const Options& options() const { return options_;}
  Texture* texture() { return tex_;}
  virtual void GenerateMips(int32_t level) = 0;
 private:
  TexturePtr tex_;
  const Options options_;
  DISALLOW_COPY_AND_ASSIGN(TextureView);
};
}  // namespace azer
