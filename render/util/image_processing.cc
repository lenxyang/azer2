#include "azer/render/util/image_processing.h"

#include "azer/render/renderer.h"
#include "azer/render/render_system.h"
#include "azer/render/geometry.h"
#include "azer/render/util/shader_util.h"

namespace azer {
const char SimpleImageProcessingEffect::kEffectName[] = 
    "SimpleImageProcessingEffect";
const azer::VertexDesc::Desc SimpleImageProcessingEffect::kVertexDesc[] = {
  {"POSITION", 0, azer::kVec4},
  {"TEXCOORD", 0, azer::kVec2},
};

const int SimpleImageProcessingEffect::kVertexDescNum = 
    arraysize(SimpleImageProcessingEffect::kVertexDesc);

SimpleImageProcessingEffect::SimpleImageProcessingEffect() {
}

const char* SimpleImageProcessingEffect::GetEffectName() const {
   return kEffectName;
}
bool SimpleImageProcessingEffect::Init(VertexDesc* desc,
                                       const ShaderPrograms& sources) {
  if (desc) {
    vertex_desc_ = desc;
  } else {
    vertex_desc_ = new azer::VertexDesc(kVertexDesc, kVertexDescNum);
  }

  DCHECK(sources.size() == azer::kRenderPipelineStageNum);
  DCHECK(!sources[azer::kVertexStage].code.empty());
  DCHECK(!sources[azer::kPixelStage].code.empty());
  InitShaders(sources);
  return true;
}

void SimpleImageProcessingEffect::ApplyGpuConstantTable(Renderer* renderer) {
}

void SimpleImageProcessingEffect::UseTexture(azer::Renderer* renderer) {
  renderer->UseTexture(azer::kPixelStage, 0, texture_.get());
}

EffectPtr CreateSimpleImageProcessingEffect() {
  Effect::ShaderPrograms shaders;
  CHECK(LoadShaderAtStage(kVertexStage, 
                          "azer/render/util/effects/hlsl/rgba2bgra.hlsl.vs",
                          &shaders));
  CHECK(LoadShaderAtStage(kPixelStage, 
                          "azer/render/util/effects/hlsl/rgba2bgra.hlsl.ps",
                          &shaders));
  EffectPtr ptr(new SimpleImageProcessingEffect);
  ptr->Init(NULL, shaders);
  return ptr;
}

// class ImageProcessing;
ImageProcessing::ImageProcessing(EffectPtr effect, TexturePtr output)
    : effect_(effect),
      output_(output) {
  Init();
}

ImageProcessing::~ImageProcessing() {
}

void ImageProcessing::Init() {
  RenderSystem* rs = RenderSystem::Current();
  Texture::Options rdopt;
  rdopt.size = output_->options().size;
  rdopt.target = (kBindTargetRenderTarget | kBindTargetShaderResource);
  renderer_ = rs->CreateRenderer(rdopt);
  Viewport viewport;
  viewport.bounds = gfx::Rect(rdopt.size);
  renderer_->SetViewport(viewport);
  GeoPlaneParams params;
  params.row = params.column = 2;
  params.row_width = params.column_width = 1.0f;
  Matrix4 mat = RotateX(Degree(90.0f));
  object_ = CreatePlaneMeshPart(effect_, params, mat);
}

void ImageProcessing::Processing() {
  Renderer* renderer = renderer_.get();
  renderer->Use();
  renderer->Clear(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
  renderer->ClearDepthAndStencil();
  object_->Render(renderer);
  Texture* tex = renderer->GetRenderTarget(0)->GetTexture();
  CHECK(tex->CopyTo(output_.get()));
}
}  // namespace azer
