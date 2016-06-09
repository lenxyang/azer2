#include <memory>

#include "dxut/dxut.h"
#include "azer/util/interactive/interactive.h"

using base::FilePath;
using base::UTF8ToUTF16;
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
  void ApplyGpuConstantTable(Renderer* renderer) override {
    {
      Matrix4 pvw = std::move(pv_ * world_);
      GpuVariable var = gpu_table_[0];
      GpuConstantsTable* tb = var.table;
      DCHECK_EQ(var.stage, kVertexStage);
      DCHECK(tb != NULL);
      tb->SetValue(0, &pvw, sizeof(Matrix4));
      tb->SetValue(1, &world_, sizeof(Matrix4));
    }
    {
      GpuVariable var = gpu_table_[1];
      GpuConstantsTable* tb = var.table;
      DCHECK(tb != NULL);
      DCHECK_EQ(var.stage, kPixelStage);
      tb->SetValue(0, &color_, sizeof(Vector4));
    }
  }
  void InitGpuConstantTable() override {
    RenderSystem* rs = RenderSystem::Current();
    // generate GpuTable init for stage kVertexStage
    GpuConstantsTable::Desc vs_table_desc[] = {
      GpuConstantsTable::Desc("pvw", GpuConstantsType::kMatrix4,
                              offsetof(vs_cbuffer, pvw), 1),
      GpuConstantsTable::Desc("world", GpuConstantsType::kMatrix4,
                              offsetof(vs_cbuffer, world), 1),
    };

    GpuVariable v;
    v.table = rs->CreateGpuConstantsTable(arraysize(vs_table_desc), vs_table_desc);
    v.stage = kVertexStage;
    v.type = kUpdatePerFrame;
    gpu_table_.push_back(v);
    // generate GpuTable init for stage kPixelStage
    GpuConstantsTable::Desc ps_table_desc[] = {
      GpuConstantsTable::Desc("color", GpuConstantsType::kVector4,
                              offsetof(ps_cbuffer, color), 1),
    };

    v.table = rs->CreateGpuConstantsTable(arraysize(ps_table_desc), ps_table_desc);
    v.stage = kPixelStage;
    v.type = kUpdatePerFrame;
    gpu_table_.push_back(v);
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
  TechSource tech(desc);
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

class MyRenderWindow : public azer::RenderWindow {
 public:
  MyRenderWindow(const gfx::Rect& rect) : azer::RenderWindow(rect) {}
  void OnInit() override;
  void OnUpdateFrame(const FrameArgs& args) override;
  void OnRenderFrame(const FrameArgs& args, Renderer* renderer) override;
 private:
  EntityPtr entity_;
  SimpleEffectPtr effect_;
  azer::Camera camera_;
  DISALLOW_COPY_AND_ASSIGN(MyRenderWindow);
};

int main(int argc, char* argv[]) {
  dxut::Env::Init(argc, argv);
  dxut::Env* env = dxut::Env::GetInstance();
  env->SetRootPath(base::FilePath(::base::UTF8ToUTF16("hlsldc")));
  gfx::Rect init_bounds(0, 0, 800, 600);
  MyRenderWindow* window(new MyRenderWindow(init_bounds));
  SampleDesc sample_desc;
  sample_desc.count = 1;
  sample_desc.quality = 0;
  window->SetSampleDesc(sample_desc);
  window->Init();
  window->Show();
  
  window->GetRenderLoop()->Run();
  return 0;
}

void MyRenderWindow::OnInit() {
  Vector3 camera_pos(0.0f, 0.0f, 5.0f);
  Vector3 lookat(0.0f, 0.0f, 0.0f);
  Vector3 up(0.0f, 1.0f, 0.0f);
  camera_->reset(camera_pos, lookat, up);

  effect_ = CreateSimpleEffect();
  Vector3 points[] = {Vector3( 0.0f, 1.0f, 0.0f),
                      Vector3(-1.0f, 0.0f, 0.0f),
                      Vector3( 1.0f, 0.0f, 0.0f)};
  entity_ = CreateGeoPointsList(kTriangleList, points, (int)arraysize(points), 
                                effect_->vertex_desc(), Matrix4::kIdentity);
}

void MyRenderWindow::OnUpdateFrame(const FrameArgs& args) {
}

void MyRenderWindow::OnRenderFrame(const FrameArgs& args, Renderer* renderer) {
  effect_->SetPV(camera().GetProjViewMatrix());
  effect_->SetWorld(Matrix4::kIdentity);
  effect_->SetColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  renderer->BindEffect(effect_);
  entity_->Draw(renderer);
}
