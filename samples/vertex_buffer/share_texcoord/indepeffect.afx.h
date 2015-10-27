 /**
 * Generated by afx
 * dont edit by hande
 */
#pragma once

#include "azer/render/render.h"
#include "azer/render/effect_creator.h"
#include "azer/math/math.h"

class IndepDiffuseEffect: public azer::Effect {
 public:
  static const char kEffectName[];
  IndepDiffuseEffect();
  ~IndepDiffuseEffect();

  const char* name() const override;
  bool Init(const ShaderPrograms& source) override;

  #pragma pack(push, 4)
  struct vs_cbuffer {
    azer::Matrix4 pvw;
  };
  #pragma pack(pop)

  void SetPVW(const azer::Matrix4& value) {
    azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &value, sizeof(azer::Matrix4));
  }
  void SetTexture(azer::Texture* tex) { tex_ = tex;}

  struct Vertex {
    azer::Vector4 position;
    azer::Vector2 texcoord;
  };

  static const int kVertexDescNum;
  static const azer::VertexDesc::Desc kVertexDesc[];
 protected:
  void ApplyGpuConstantTable(azer::Renderer* renderer) override;
  void InitTechnique(const ShaderPrograms& source);
  void InitGpuConstantTable();
  void UseTexture(azer::Renderer* renderer) override;

  azer::TexturePtr tex_;
  DISALLOW_COPY_AND_ASSIGN(IndepDiffuseEffect);
};

typedef scoped_refptr<IndepDiffuseEffect> IndepDiffuseEffectPtr;
IndepDiffuseEffectPtr CreateDiffuse();