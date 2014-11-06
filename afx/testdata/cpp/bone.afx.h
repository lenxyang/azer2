 /**
 * Generated by afx
 * dont edit by hande
 */
#pragma once
#include "azer/render/render.h"
#include "azer/math/math.h"


class HaredwareSkinnedMeshEffect: public azer::Effect {
 public:
  HaredwareSkinnedMeshEffect(const std::vector<std::string>& sources, azer::RenderSystem* rs);
  ~HaredwareSkinnedMeshEffect();
 /**
   * uniform buffer
   */
  #pragma pack(push, 4)
  struct vs_cbuffer {
    azer::Matrix4 bone[100];
    azer::Matrix4 world;
    azer::Matrix4 pv;
  };
  #pragma pack(pop)

  void SetBones(const azer::Matrix4* value, int num) {
    azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(0, value, sizeof(azer::Matrix4) * num);
  }
  void SetWorld(const azer::Matrix4& value) {
    azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(1, &value, sizeof(azer::Matrix4));
  }
  void SetProjView(const azer::Matrix4& value) {
    azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(2, &value, sizeof(azer::Matrix4));
  }
  void SetDiffuseTex(const azer::TexturePtr& texture) {
    ps_tex_ = texture;
  }


  /**
  * struct of Vertex 
  * input of Vertex Shader
  */
  struct Vertex {
    azer::Vector4 position;
    azer::Vector2 coordtex;
    azer::Vector4 normal;
    azer::IntVec4 index;
    azer::Vector4 weights;
    Vertex(){}
    Vertex(const azer::Vector4 p0, const azer::Vector2 p1, const azer::Vector4 p2, const azer::IntVec4 p3, const azer::Vector4 p4)
      : position(p0)
      , coordtex(p1)
      , normal(p2)
      , index(p3)
      , weights(p4)
      {}
  };

  azer::VertexDescPtr GetVertexDesc() { return vertex_desc_ptr_;}
  static const int kVertexDescNum;
  static const azer::VertexDesc::Desc kVertexDesc[];
 protected:
  void Init();
  void InitTechnique();
  virtual void UseTexture(azer::Renderer* renderer) OVERRIDE;
  virtual void SetMaterial(azer::MaterialPtr material) OVERRIDE;
  const std::vector<std::string>& sources_;

  azer::TexturePtr ps_tex_;

  azer::VertexDescPtr vertex_desc_ptr_;
  DISALLOW_COPY_AND_ASSIGN(HaredwareSkinnedMeshEffect);
};
