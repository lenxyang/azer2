#include "azer/render_system/d3d11/overlay.h"

#include "azer/render/render.h"
#include "azer/render_system/d3d11/render_system.h"
#include "azer/render/reusable_object.h"
#include "azer/render/reusable_object_util.h"
#include "azer/render/render_state_autorestore.h"

namespace azer {
namespace d3d11 {
namespace {
#pragma pack(push, 4)
struct vs_cbuffer {
  Matrix4 transform;
  Vector4 vertex[4];
  Vector4 texcoord[4];
};
#pragma pack(pop)
}  // namespace

const char* D3DOverlayEffect::kVertexShaderProg = ""
    "struct VS_OUTPUT {                                  \n"
    "  float4 pos      : SV_POSITION;                    \n"
    "  float2 texcoord : TEXCOORD0;                      \n"
    "};                                                  \n"
    "cbuffer c_buffer {                                  \n"
    "  float4x4 transform;                               \n"
    "  float4   vertex[4];                               \n"
    "  float4   texcoord[4];                             \n"
    "};                                                  \n"
    "VS_OUTPUT vs_main(int index : INDEX) {              \n"
    "  VS_OUTPUT output;                                 \n"
    "  float4 position = vertex[index];                  \n"
    "  output.pos = mul(transform, position);            \n"
    "  output.texcoord = texcoord[index].xy;             \n"
    "  return output;                                    \n"
    "}";


const char* D3DOverlayEffect::kPixelShaderProg = ""
    "struct VS_OUTPUT {                                      \n"
    "  float4 pos      : SV_POSITION;                        \n"
    "  float2 texcoord : TEXCOORD0;                          \n"
    "};                                                      \n"
    "Texture2D ObjTexture;                                   \n"
    "SamplerState ObjSamplerState;                           \n"
    "float4 ps_main(VS_OUTPUT input) : SV_TARGET {           \n"
    "  float4 diffuse =  ObjTexture.Sample(ObjSamplerState, input.texcoord); \n"
    "  return diffuse;                                                  \n"
    "}";


void D3DOverlayEffect::Use(Renderer* renderer) {
  DCHECK(texture_.get() != NULL);
  renderer->UseTexture(azer::kPixelStage, 0, texture_.get());
  UseConstantsTable(renderer);
  UseTechnique(renderer);
}

void D3DOverlayEffect::SetTransform(const Matrix4& matrix) {
  azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(0, &matrix, sizeof(azer::Matrix4));
}

void D3DOverlayEffect::SetVertex(const Vector4 vertex[4]) {
  azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(1, vertex, sizeof(Vector4) * 4);
}

void D3DOverlayEffect::SetTexcoord(const Vector4 texcoord[4]) {
  azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
  DCHECK(tb != NULL);
  tb->SetValue(2, texcoord, sizeof(Vector4) * 4);
}

bool D3DOverlayEffect::Init(Overlay* overlay, D3DRenderSystem* rs) {
  // generate GpuTable init for stage azer::kVertexStage
  azer::GpuConstantsTable::Desc vs_table_desc[] = {
    azer::GpuConstantsTable::Desc("transform", azer::GpuConstantsType::kMatrix4,
                                  offsetof(vs_cbuffer, transform), 1),
    azer::GpuConstantsTable::Desc("vertex", azer::GpuConstantsType::kVector4,
                                  offsetof(vs_cbuffer, vertex), 4),
    azer::GpuConstantsTable::Desc("texcoord", azer::GpuConstantsType::kVector4,
                                  offsetof(vs_cbuffer, texcoord), 4),
  };
  gpu_table_[azer::kVertexStage] = render_system_->CreateGpuConstantsTable(
      arraysize(vs_table_desc), vs_table_desc);

  technique_ = rs->CreateTechnique();
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
  effect_ = new D3DOverlayEffect(render_system_);
  if (effect_->Init(this, render_system_)) {
    return true;
  } else {
    effect_ = NULL;
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
  {"INDEX",    0, kInt},
};

const int D3DOverlay::kVertexDescNum = arraysize(D3DOverlay::kVertexDesc);

bool D3DOverlay::InitVertex(RenderSystem* rs) {
  // create vertex buffer
  const int32 indices[] = {2, 1, 0, 2, 0, 3}; 
  vertex_desc_ptr_ = new VertexDesc(kVertexDesc, kVertexDescNum);
  scoped_refptr<VertexData> data(new VertexData(vertex_desc_ptr_, kVertexNum));
  int32* ptr = (int32*)data->pointer();
  memcpy(ptr, indices, sizeof(indices));
  vb_ptr_ = rs->CreateVertexBuffer(VertexBuffer::Options(), data.get());
  if (!vb_ptr_.get()) {
    return false;
  }

  return true;
}

void D3DOverlay::Render(Renderer* renderer) {
  SetBlending(renderer);
  azer::Vector2 texcoord[4];
  
  DCHECK (effect_.get() != NULL);
  AutoCullingMode(kCullNone, renderer);
  effect_->SetTransform(transform_);
  effect_->SetVertex(vertex_);
  effect_->SetTexcoord(texcoord_);
  effect_->SetTexture(tex_);
  effect_->Use(renderer);
  renderer->Draw(vb_ptr_.get(), azer::kTriangleList, 6);
  ResetBlending(renderer);
}
}  // namespace d3d11
}  // namespace azer
