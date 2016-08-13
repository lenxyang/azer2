#include "azer/azer.h"

using namespace azer;
class SimpleEffect : public Effect {
 public:
  static const char kEffectName[];
  SimpleEffect() {
    world_ = Matrix4::kIdentity;
    color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
  }
  ~SimpleEffect() {}

  const char* GetEffectName() const override { return kEffectName;}

#pragma pack(push, 4)
  struct vs_cbuffer {
    Matrix4 pvw;
    Matrix4 world;
  };

  struct ps_cbuffer {
    Vector4 color;
  };
#pragma pack(pop)

  void SetPV(const Matrix4& value) { pv_ = value;}
  void SetWorld(const Matrix4& value) { world_ = value;}
  void SetColor(const Vector4& value) {color_ = value;}
 protected:
  void ApplyShaderParamTable(Renderer* renderer) override {
    {
      Matrix4 pvw = std::move(pv_ * world_);
      ShaderParamTable* tb = GetShaderClosure(kVertexStage)->table_at(0);
      DCHECK(tb != NULL);
      tb->SetValue(0, &pvw, sizeof(Matrix4));
      tb->SetValue(1, &world_, sizeof(Matrix4));
    }
    {
      ShaderParamTable* tb = GetShaderClosure(kPixelStage)->table_at(0);
      DCHECK(tb != NULL);
      tb->SetValue(0, &color_, sizeof(Vector4));
    }
  }

  ShaderClosurePtr InitVertexStage(Shader* shader) {
    ShaderClosurePtr closure(new ShaderClosure(shader->stage()));
    closure->SetShader(shader, 0, 0, 0);
    // generate GpuTable init for stage kVertexStage
    ShaderParamTable::Desc vs_table_desc[] = {
      ShaderParamTable::Desc("pvw", ShaderParamType::kMatrix4,
                             offsetof(vs_cbuffer, pvw), 1),
      ShaderParamTable::Desc("world", ShaderParamType::kMatrix4,
                             offsetof(vs_cbuffer, world), 1),
    };

    ShaderParamTablePtr table;
    table = new ShaderParamTable(arraysize(vs_table_desc), vs_table_desc);
    closure->SetShader(shader, 1, 0, 0);
    closure->SetShaderParamTable(0, table.get());
    return closure;
  }

  ShaderClosurePtr InitPixelStage(Shader* shader) {
    ShaderClosurePtr closure(new ShaderClosure(shader->stage()));
    closure->SetShader(shader, 0, 0, 0);
    // generate GpuTable init for stage kPixelStage
    ShaderParamTable::Desc ps_table_desc[] = {
      ShaderParamTable::Desc("color", ShaderParamType::kVector4,
                             offsetof(ps_cbuffer, color), 1),
    };

    ShaderParamTablePtr table;
    table = new ShaderParamTable(arraysize(ps_table_desc), ps_table_desc);
    closure->SetShader(shader, 1, 0, 0);
    closure->SetShaderParamTable(0, table.get());
    return closure;
  }

  Matrix4 pv_;
  Matrix4 world_;
  Vector4 color_;
  DISALLOW_COPY_AND_ASSIGN(SimpleEffect);
};
const char SimpleEffect::kEffectName[] = "SimpleEffect";

typedef scoped_refptr<SimpleEffect> SimpleEffectPtr;
SimpleEffectPtr CreateSimpleEffect() {
  // class PositionVertex
  const VertexDesc::Desc kVertexDesc[] = {
    {"POSITION", 0, kVec4},
  };
  VertexDescPtr desc(new VertexDesc(kVertexDesc, arraysize(kVertexDesc)));
  TechSource tech(desc.get());
  ShaderInfo shader;
  shader.path = "effect.vs";
  shader.stage = kVertexStage;
  shader.code = ""
      "#pragma pack_matrix(row_major)\n"
      "struct VsOutput {\n"
      "  float4 position:SV_POSITION;\n"
      "}\n;"
      "struct VSInput {\n"
      "  float4 position:POSITION;\n"
      "};\n"
      "cbuffer c_buffer {\n"
      "  float4x4 pvw;"
      "  float4x4 world;"
      "};"
      "VsOutput vs_main(VSInput input) {\n"
      "VsOutput o;"
      "o.position = mul(pvw, input.position);"
      "return o;"
      "}";
  tech.AddShader(shader);
  shader.path = "effect.ps";
  shader.stage = kPixelStage;
  shader.code = "#pragma pack_matrix(row_major)\n"
      "struct VsOutput {\n"
      "  float4 position:SV_POSITION;\n"
      "};\n"
      "cbuffer c_buffer {\n"
      "  float4 color;\n"
      "};\n"
      "float4 ps_main(VsOutput o):SV_TARGET {\n"
      "  return color;"
      "}\n";
  tech.AddShader(shader);
  SimpleEffectPtr ptr(new SimpleEffect);
  ptr->Init(tech);
  return ptr;
}
