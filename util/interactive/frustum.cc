#include "azer/util/interactive/frustum.h"

#include "azer/effect/color_effect.h"
#include "azer/render/camera.h"
#include "azer/render/render_system.h"
#include "azer/util/geometry/box.h"
#include "azer/util/interactive/env.h"

namespace azer {

EntityDataPtr CreateFrustumBox(VertexDesc* desc, const Camera& camera) {
  Vector3 pos[8];
  CalcCameraBundingPos(camera, pos);
  Vector4 posvec[8];
  for (int i = 0; i < arraysize(pos); ++i) {
    posvec[i] = Vector4(pos[i], 1.0f);
  }

  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata.get(), idata.get()));
  AppendGeoHexaHedronFrameData(data.get(), posvec, Matrix4::kIdentity);
  AppendGeoHexaHedronData(data.get(), posvec, Matrix4::kIdentity);
  return data;
}

EntityDataPtr CreateFrustumBoxByMatrix(VertexDesc* desc, const Matrix4& pvw) {
  Vector4 posvec[8];
  Vector4 projvec[8] = {
    Vector4(-1.0f,  1.0f, 0.0f, 1.0f),
    Vector4( 1.0f,  1.0f, 0.0f, 1.0f),
    Vector4( 1.0f, -1.0f, 0.0f, 1.0f),
    Vector4(-1.0f, -1.0f, 0.0f, 1.0f),
    Vector4(-1.0f,  1.0f, 1.0f, 1.0f),
    Vector4( 1.0f,  1.0f, 1.0f, 1.0f),
    Vector4( 1.0f, -1.0f, 1.0f, 1.0f),
    Vector4(-1.0f, -1.0f, 1.0f, 1.0f),
  };
  Matrix4 inv = pvw.InverseCopy();
  for (int i = 0; i < arraysize(projvec); ++i) {
    posvec[i] = std::move(inv * projvec[i]);
  }

  VertexDataPtr vdata(new VertexData(desc, 1));
  IndicesDataPtr idata(new IndicesData(1));
  EntityDataPtr data(new EntityData(vdata.get(), idata.get()));
  AppendGeoHexaHedronFrameData(data.get(), posvec, Matrix4::kIdentity);
  AppendGeoHexaHedronData(data.get(), posvec, Matrix4::kIdentity);
  return data;
}

FrustumBox::FrustumBox()
    : color_(Vector3(0.4f, 0.0f, 0.0f)),
      alpha_(0.2f) {
  RenderSystem* rs = RenderSystem::Current();
  rasterizer_ptr_ = rs->CreateRasterizerState();
  rasterizer_ptr_->SetCullingMode(kCullNone);

  Blending::BlendDesc blend_desc;
  blend_desc.desc[0].src = Blending::kSrcAlpha;
  blend_desc.desc[0].dest = Blending::kSrcInvAlpha;
  blend_desc.desc[0].oper = Blending::kAdd;
  blend_desc.desc[0].src_alpha = Blending::kOne;
  blend_desc.desc[0].dest_alpha = Blending::kZero;
  blend_desc.desc[0].alpha_oper = Blending::kAdd;
  blend_desc.desc[0].mask = Blending::kWriteColor;
  blend_desc.desc[0].enable = true;
  blend_desc.alpha_to_converage = false;
  blending_ = rs->CreateBlending(blend_desc);

  InteractiveEnv* env = InteractiveEnv::GetInstance();
  color_effect_ = (AmbientColorEffect*)env->GetEffect("AmbientColorEffect");
}
void FrustumBox::Update(const Camera& camera, float zfar) {
  float farval = (zfar > 0 ? zfar : camera.frustum().zfar());
  Camera newcamera = camera;
  newcamera.mutable_frustum()->set_far(farval);
  EntityDataPtr data = CreateFrustumBox(color_effect_->vertex_desc(), newcamera);
  frustum_box_ = new Entity(data.get());
}

void FrustumBox::Update(const Matrix4& pvw) {
  EntityDataPtr data = CreateFrustumBoxByMatrix(color_effect_->vertex_desc(), pvw);
  frustum_box_ = new Entity(data.get());
}

void FrustumBox::Render(const Camera& camera, Renderer* renderer) {
  if (frustum_box_.get()) {
    ScopedRasterizerState scoped_raster(renderer);
    ScopedResetBlending scoped_blending(renderer);
    renderer->SetRasterizerState(rasterizer_ptr_.get());
    renderer->SetBlending(blending_.get(), 0, 0xffffffff);
    color_effect_->SetPV(camera.GetProjViewMatrix());
    color_effect_->SetWorld(Matrix4::kIdentity);
    color_effect_->SetAmbient(Vector4(color_, 1.0f));
    renderer->BindEffect(color_effect_.get());
    frustum_box_->DrawSub(0, renderer);

    color_effect_->SetAmbient(Vector4(color_, alpha_));
    renderer->BindEffect(color_effect_.get());
    frustum_box_->DrawSub(1, renderer);
  }
}
}  // namespace azer
