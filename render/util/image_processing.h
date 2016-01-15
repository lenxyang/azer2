#pragma once

#include "azer/render/effect.h"
#include "azer/render/texture.h"
#include "azer/render/render_target.h"
#include "azer/render/mesh.h"
#include "azer/render/renderer.h"

namespace azer {

class SimpleImageProcessingEffect : public Effect {
 public:
  static const char kEffectName[];
  SimpleImageProcessingEffect();

  const char* GetEffectName() const override;
  bool Init(VertexDesc* desc, const ShaderPrograms& source) override;
  void SetInputTex(TexturePtr texture) { texture_ = texture;}

  struct Vertex {
    Vector4 position;
    Vector2 texcoord;
  };
  static const int kVertexDescNum;
  static const azer::VertexDesc::Desc kVertexDesc[];
 private:
  void UseTexture(azer::Renderer* renderer) override;
  void ApplyGpuConstantTable(Renderer* renderer) override;
  TexturePtr texture_;
  DISALLOW_COPY_AND_ASSIGN(SimpleImageProcessingEffect);
};

EffectPtr CreateSimpleImageProcessingEffect();

class ImageProcessing : public ::base::RefCounted<ImageProcessing> {
 public:
  ImageProcessing(EffectPtr effect, TexturePtr output);
  ~ImageProcessing();

  EffectPtr GetEffect() { return effect_;}
  void Processing();
 private:
  void Init();
  TexturePtr input_;
  TexturePtr output_;
  RendererPtr renderer_;
  MeshPartPtr object_;
  EffectPtr effect_;
  DISALLOW_COPY_AND_ASSIGN(ImageProcessing);
};

typedef scoped_refptr<ImageProcessing> ImageProcessingPtr;
}  // namespace azer
