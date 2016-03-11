#include "azer/resource/geometry_loader.h"

#include "azer/resource/mesh_loader.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/azer.h"
#include "azer/resource/mesh_load_util.h"
#include "azer/resource/resource_util.h"
#include "azer/render/geometry.h"

namespace azer {

const char GeometryLoader::kSpecialLoaderName[] = "azer::GeometryLoader";
GeometryLoader::GeometryLoader() {}

GeometryLoader::~GeometryLoader() {}

const char* GeometryLoader::GetLoaderName() const {
  return kSpecialLoaderName;
}
VariantResource GeometryLoader::Load(const ConfigNode* node,
                                     ResourceLoadContext* ctx) {
  ConfigNode* vertex_desc_node = GetTypedReferNode("vertex_desc", node);
  VertexDescPtr vertex_desc = LoadReferVertexDesc(vertex_desc_node, ctx);
  ConfigNode* effect_node = GetTypedReferNode("effect", node);
  EffectPtr effect = LoadReferEffect(effect_node, ctx);
  ConfigNode* material_node = GetTypedReferNode("material", node);
  BaseMaterialPtr material;
  if (material_node) {
    material = LoadReferMaterial(material_node, ctx);
  }

  if (!vertex_desc.get() || !effect.get()) {
    return VariantResource();
  }

  MeshPartPtr part;
  std::string geometry_type  = node->GetAttr("geotype");
  if (geometry_type == "sphere") {
    part = CreateSphere(node, effect, ctx);
  } else if (geometry_type == "box") {
    part = CreateBox(node, effect, ctx);
  } else if (geometry_type == "plane") {
    part = CreatePlane(node, effect, ctx);
  } else if (geometry_type == "round") {
    part = CreateRound(node, effect, ctx);
  } else if (geometry_type == "cone") {
    part = CreateCone(node, effect, ctx);
  } else if (geometry_type == "barrel") {
    part = CreateBarrel(node, effect, ctx);
  } else if (geometry_type == "cylinder") {
    part = CreateCylinder(node, effect, ctx);
  } else {
    CHECK(false);
  }
  CHECK(part.get());
  VariantResource resource;
  resource.type = kResTypeMesh;
  resource.mesh = new Mesh;
  resource.mesh->AddMeshPart(part); 
  resource.retcode = 0;
  if (material.get())
    resource.mesh->AddProvider(material);
  return resource;
}

MeshPartPtr GeometryLoader::CreateSphere(const ConfigNode* node, Effect* e,
                                         ResourceLoadContext* ) {
  GeoSphereParams params;
  if (node->HasAttr("radius")) {
    CHECK(node->GetAttrAsFloat("radius", &params.radius));
  }
  if (node->HasAttr("stack")) {
    CHECK(node->GetAttrAsInt("stack", &params.stack));
  }
  if (node->HasAttr("slice")) {
    CHECK(node->GetAttrAsInt("slice", &params.slice));
  }
  MeshPartPtr ptr = CreateSphereMeshPart(e, params, Matrix4::kIdentity);
  return ptr;
}

MeshPartPtr GeometryLoader::CreateBox(const ConfigNode* node,
                                      Effect* e, ResourceLoadContext* ) {
  MeshPartPtr ptr = CreateBoxMeshPart(e, Matrix4::kIdentity);
  return ptr;
}

MeshPartPtr GeometryLoader::CreatePlane(const ConfigNode* node,
                                        Effect* e, ResourceLoadContext* ) {
  GeoPlaneParams params;
  if (node->HasAttr("row_width")) {
    CHECK(node->GetAttrAsFloat("row_width", &params.row_width));
  }
  if (node->HasAttr("row")) {
    CHECK(node->GetAttrAsInt("row", &params.row));
  }
  if (node->HasAttr("column_width")) {
    CHECK(node->GetAttrAsFloat("column_width", &params.column_width));
  }
  if (node->HasAttr("column")) {
    CHECK(node->GetAttrAsInt("column", &params.column));
  }
  return CreatePlaneMeshPart(e, params, Matrix4::kIdentity);
}

MeshPartPtr GeometryLoader::CreateCone(const ConfigNode* node,
                                       Effect* e, ResourceLoadContext* ) {
  GeoConeParams params;
  if (node->HasAttr("radius")) {
    CHECK(node->GetAttrAsFloat("radius", &params.radius));
  }
  if (node->HasAttr("slice")) {
    CHECK(node->GetAttrAsInt("slice", &params.slice));
  }
  if (node->HasAttr("height")) {
    CHECK(node->GetAttrAsFloat("height", &params.height));
  }
  return CreateConeMeshPart(e, params, Matrix4::kIdentity);
}

MeshPartPtr GeometryLoader::CreateRound(const ConfigNode* node,
                                        Effect* e, 
                                        ResourceLoadContext* ) {
  int32 slice = 24;;
  float radius = 1.0f;
  if (node->HasAttr("radius")) {
    CHECK(node->GetAttrAsFloat("radius", &radius));
  }
  if (node->HasAttr("slice")) {
    CHECK(node->GetAttrAsInt("slice", &slice));
  }
  if (node->GetAttr("wireframe") == "true") {
    return CreateRoundMeshPart(e, radius, slice, Matrix4::kIdentity);
  } else {
    return CreateCircleMeshPart(e, radius, slice, Matrix4::kIdentity);
  }
}

MeshPartPtr GeometryLoader::CreateCylinder(const ConfigNode* node,
                                           Effect* e, 
                                           ResourceLoadContext* ) {
  GeoBarrelParams params;
  if (node->HasAttr("bottom_radius")) {
    CHECK(node->GetAttrAsFloat("bottom_radius", &params.bottom_radius));
  }
  if (node->HasAttr("top_radius")) {
    CHECK(node->GetAttrAsFloat("top_radius", &params.top_radius));
  }
  if (node->HasAttr("slice")) {
    CHECK(node->GetAttrAsInt("slice", &params.slice));
  }
  if (node->HasAttr("stack")) {
    CHECK(node->GetAttrAsInt("stack", &params.stack));
  }
  if (node->HasAttr("height")) {
    CHECK(node->GetAttrAsFloat("height", &params.height));
  }
  return CreateCylinderMeshPart(e, params, Matrix4::kIdentity);
}

MeshPartPtr GeometryLoader::CreateBarrel(const ConfigNode* node,
                                         Effect* e, 
                                         ResourceLoadContext* ) {
  GeoBarrelParams params;
  if (node->HasAttr("bottom_radius")) {
    CHECK(node->GetAttrAsFloat("bottom_radius", &params.bottom_radius));
  }
  if (node->HasAttr("top_radius")) {
    CHECK(node->GetAttrAsFloat("top_radius", &params.top_radius));
  }
  if (node->HasAttr("slice")) {
    CHECK(node->GetAttrAsInt("slice", &params.slice));
  }
  if (node->HasAttr("stack")) {
    CHECK(node->GetAttrAsInt("stack", &params.stack));
  }
  if (node->HasAttr("height")) {
    CHECK(node->GetAttrAsFloat("height", &params.height));
  }
  return CreateBarrelMeshPart(e, params, Matrix4::kIdentity);
}

MeshPartPtr GeometryLoader::CreateTour(const ConfigNode* node, Effect* e, 
                                       ResourceLoadContext* ) {
  return MeshPartPtr();
}

bool GeometryLoader::CouldLoad(ConfigNode* node) const {
  return node->tagname() == "geometry";
}

}  // namespace azer

