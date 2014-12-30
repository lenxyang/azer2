 /**
 * Generated by afx
 * dont edit by hande
 */
#pragma once

#include "azer/render/render.h"
#include "azer/math/math.h"



class effect: public azer::Effect {
 public:
  effect(const std::vector<std::string>& sources, azer::RenderSystem* rs);
  ~effect();
 /**
   * uniform buffer
   */
  #pragma pack(push, 4)
  struct afx__SpotLight{
    azer::Vector4 position;
    azer::Vector4 direction;
    azer::Vector4 diffuse;
  };
  struct Material{
    azer::Texture difftex;
  };

  struct ps_cbuffer {
    afx__SpotLight light1;
    Material mtrl;
  };
  #pragma pack(pop)

  void SetLight(const afx__SpotLight& value) {
    azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kPixelStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &value, sizeof(afx__SpotLight));
  }
  void set_ps_mtrl(const Material& value) {
    azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kPixelStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(1, &value, sizeof(Material));
  }
  void set_ps_mtrl__difftex_(int index, const azer::TexturePtr& texture) {
    DCHECK_LT(index, (int)arraysize(ps_mtrl__difftex_));
    ps_mtrl__difftex_[index] = texture;
  }


  /**
  * struct of Vertex 
  * input of Vertex Shader
  */
  struct Vertex {
    azer::Vector3 position;
    azer::Vector3 normal;
    azer::Vector3 texcoord;
    Vertex(){}
    Vertex(const azer::Vector3 p0, const azer::Vector3 p1, const azer::Vector3 p2)
      : position(p0)
      , normal(p1)
      , texcoord(p2)
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

  azer::TexturePtr ps_mtrl__difftex_[3];

  azer::VertexDescPtr vertex_desc_ptr_;
  DISALLOW_COPY_AND_ASSIGN(effect);
};