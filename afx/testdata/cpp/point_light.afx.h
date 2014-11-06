 /**
 * Generated by afx
 * dont edit by hande
 */
#pragma once
#include "azer/render/render.h"
#include "azer/math/math.h"


class PoingLightEffect: public azer::Effect {
 public:
  PoingLightEffect(const std::vector<std::string>& sources, azer::RenderSystem* rs);
  ~PoingLightEffect();
 /**
   * uniform buffer
   */
  #pragma pack(push, 4)

  struct vs_cbuffer {
    azer::Matrix4 pvw;
    azer::Matrix4 world;
  };
  struct PointLight{
    azer::Vector4 direction;
    azer::Vector4 diffuse;
  };

  struct ps_cbuffer {
    PointLight light;
    azer::Vector4 diffuse;
  };
  #pragma pack(pop)

  void SetPVW(const azer::Matrix4& value) {
    azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &value, sizeof(azer::Matrix4));
  }
  void SetWorld(const azer::Matrix4& value) {
    azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kVertexStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(1, &value, sizeof(azer::Matrix4));
  }
  void SetDiffuse(const azer::Vector4& value) {
    azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kPixelStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(1, &value, sizeof(azer::Vector4));
  }
  void SetPointLight(const PointLight& value) {
    azer::GpuConstantsTable* tb = gpu_table_[(int)azer::kPixelStage].get();
    DCHECK(tb != NULL);
    tb->SetValue(0, &value, sizeof(PointLight));
  }


  /**
  * struct of Vertex 
  * input of Vertex Shader
  */
  struct Vertex {
    azer::Vector4 position;
    azer::Vector2 coordtex;
    azer::Vector4 normal;
    Vertex(){}
    Vertex(const azer::Vector4 p0, const azer::Vector2 p1, const azer::Vector4 p2)
      : position(p0)
      , coordtex(p1)
      , normal(p2)
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


  azer::VertexDescPtr vertex_desc_ptr_;
  DISALLOW_COPY_AND_ASSIGN(PoingLightEffect);
};
