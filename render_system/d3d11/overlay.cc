#include "azer/render_system/d3d11/overlay.h"

#include "azer/render/render.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render/reusable_object.h"
#include "azer/render/reusable_object_util.h"

namespace azer {
namespace d3d11 {

const char* D3DOverlayEffect::kVertexShaderProg = ""
    "struct VS_OUTPUT {                                  \n"
    "  float4 Pos : SV_POSITION;                         \n"
    "  float2 texcoord : TEXCOORD;                       \n"
    "};                                                  \n"
    "VS_OUTPUT vs_main(float4 inpos : POSITION,          \n"
    "                  float2 tex : TEXCOORD) {          \n"
    "  VS_OUTPUT output;                                 \n"
    "  output.Pos = inpos;                               \n"
    "  inpos.z = 0.0f;                                   \n"
    "  output.texcoord = tex;                            \n"
    "  return output;                                    \n"
    "}";


const char* D3DOverlayEffect::kPixelShaderProg = ""
    "struct VS_OUTPUT {                                      \n"
    "  float4 Pos : SV_POSITION;                             \n"
    "  float2 Tex : TEXTURE;                                 \n"
    "};                                                      \n"
    "Texture2D ObjTexture;                                   \n"
    "SamplerState ObjSamplerState;                           \n"
    "float4 ps_main(VS_OUTPUT input) : SV_TARGET {           \n"
    "  float4 diffuse =  ObjTexture.Sample(ObjSamplerState, input.Tex); \n"
    "  clip(diffuse.a - 0.01);                                          \n"
    "  return diffuse;                                                  \n"
    "}";


void D3DOverlayEffect::Use(Renderer* renderer) {
  DCHECK(texture_.get() != NULL);
  renderer->UseTexture(azer::kPixelStage, 0, texture_.get());
  UseConstantsTable(renderer);
  UseTechnique(renderer);
}

bool D3DOverlayEffect::Init(Overlay* overlay, D3DRenderSystem* rs) {
  technique_.reset(rs->CreateTechnique());
  VertexDescPtr& desc = overlay->GetVertexDesc();
  ReusableObject* object = rs->GetReusableObject();
  
  const char* overlay_vs = "azer_overlay_vs";
  const char* overlay_ps = "azer_overlay_ps";
  GpuProgramPtr vs(GetVertexProgramMayCreate(overlay_vs, kVertexShaderProg, desc));
  GpuProgramPtr ps(GetPixelProgramMayCreate(overlay_ps, kPixelShaderProg));
  if (vs.get() && ps.get()) {
    technique_->AddGpuProgram(vs);
    technique_->AddGpuProgram(ps);
    return true;
  } else {
    return false;
  }
}

bool D3DOverlay::InitEffect() {
  DCHECK(render_system_ != NULL);
  effect_.reset(new D3DOverlayEffect(render_system_));
  if (effect_->Init(this, render_system_)) {
    return true;
  } else {
    effect_.reset(NULL);
    return false;
  }
}

bool D3DOverlay::Init(azer::RenderSystem* rs) {
  if (!InitVertex(rs)) {
    return false;
  }

  if (!InitEffect()) {
    return false;
  }

  return Overlay::Init(rs);
}

const VertexDesc::Desc D3DOverlay::kVertexDesc[] = {
  {"POSITION", 0, kVec4},
  {"TEXCOORD", 0, kVec2},
  {"INDEX",    0, kInt},
};

const int D3DOverlay::kVertexDescNum = arraysize(D3DOverlay::kVertexDesc);

bool D3DOverlay::InitVertex(RenderSystem* rs) {
  // create vertex buffer
  vertex_desc_ptr_.reset(new VertexDesc(kVertexDesc, kVertexDescNum));
  VertexData data(vertex_desc_ptr_, kVertexNum);
  Vertex* ptr = (Vertex*)data.pointer();
  ptr->position = azer::Vector4(rect_.x(), rect_.bottom(), 0.0f, 1.0f);
  ptr->texcoord = azer::Vector2(0.0f, 1.0f);
  ptr->index = 0;
  ptr++;
  ptr->position = azer::Vector4(rect_.x(), rect_.y(), 0.0f, 1.0f);
  ptr->texcoord = azer::Vector2(0.0f, 0.0f);
  ptr->index = 1;
  ptr++;
  ptr->position = azer::Vector4(rect_.right(), rect_.bottom(), 0.0f, 1.0f);
  ptr->texcoord = azer::Vector2(1.0f, 1.0f);
  ptr->index = 2;
  ptr++;

  ptr->position = azer::Vector4(rect_.right(), rect_.bottom(), 0.0f, 1.0f);
  ptr->texcoord = azer::Vector2(1.0f, 1.0f);
  ptr->index = 3;
  ptr++;
  ptr->position = azer::Vector4(rect_.x(), rect_.y(), 0.0f, 1.0f);
  ptr->texcoord = azer::Vector2(0.0f, 0.0f);
  ptr->index = 4;
  ptr++;
  ptr->position = azer::Vector4(rect_.right(), rect_.y(), 0.0f, 1.0f);
  ptr->texcoord = azer::Vector2(1.0f, 0.0f);
  ptr->index = 5;

  vb_ptr_.reset(rs->CreateVertexBuffer(VertexBuffer::Options(), &data));
  if (!vb_ptr_.get()) {
    return false;
  }

  return true;
}

void D3DOverlay::Render(Renderer* renderer) {
  SetBlending(renderer);
  DCHECK (effect_.get() != NULL);
  effect_->Use(renderer);
  renderer->Draw(vb_ptr_.get(), azer::kTriangleList, 6);
  ResetBlending(renderer);
}
}  // namespace d3d11
}  // namespace azer
