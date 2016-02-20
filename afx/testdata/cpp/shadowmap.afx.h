 /**
 * Generated by afx
 * dont edit by hande
 */
#pragma once

#include "azer/render/render.h"
#include "azer/math/math.h"



class DiffuseEffect: public azer::Effect {
 public:
  DiffuseEffect(const std::vector<std::string>& sources, azer::RenderSystem* rs);
  ~DiffuseEffect();
 /**
   * uniform buffer
   */
  #pragma pack(push, 4)

  struct vs_cbuffer {
    azer::Matrix4 pvw;
    azer::Matrix4 shadowmap_pvw;
  };
  #pragma pack(pop)

  void SetPVW(const azer::Matrix4& value) {
    azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &value, sizeof(azer::Matrix4));
  }
  void SetShadowPVW(const azer::Matrix4& value) {
    azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(1, &value, sizeof(azer::Matrix4));
  }
  void SetShadowmapTexture(const azer::TexturePtr& texture) {
    ps_shadowmap_tex_ = texture;
  }


  /**
  * struct of Vertex 
  * input of Vertex Shader
  */
  struct Vertex {
    azer::Vector3 position;
    Vertex(){}
    Vertex(const azer::Vector3 p0)
      : position(p0)
      {}
  };

  azer::VertexDescPtr GetVertexDesc() { return vertex_desc_ptr_;}
  static const int kVertexDescNum;
  static const azer::VertexDesc::Desc kVertexDesc[];
 protected:
  void Init();
  void InitTechnique();
  virtual void UseTexture(azer::Renderer* renderer) override;
  const std::vector<std::string>& sources_;

  azer::TexturePtr ps_shadowmap_tex_;

  azer::VertexDescPtr vertex_desc_ptr_;
  DISALLOW_COPY_AND_ASSIGN(DiffuseEffect);
};
