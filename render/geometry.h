#pragma once

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "azer/base/export.h"
#include "azer/math/matrix4.h"
#include "azer/render/common.h"

namespace azer {
class Effect;
class Entity;
class MeshPart;
class IndicesData;
class VertexData;
class Vector3;
class VertexDesc;

typedef scoped_refptr<Entity> EntityPtr;
typedef scoped_refptr<MeshPart> MeshPartPtr;

// sphere
struct AZER_EXPORT GeoSphereParams {
  GeoSphereParams() : radius(1.0f), stack(64), slice(64) {}
  float radius;
  int32 stack;
  int32 slice;
};

AZER_EXPORT EntityPtr CreateSphereEntity(
    VertexDesc* , const GeoSphereParams&, const Matrix4& mat);
AZER_EXPORT EntityPtr CreateSphereFrameEntity(
    VertexDesc*,const GeoSphereParams&, const Matrix4& mat);
AZER_EXPORT MeshPartPtr CreateSphereMeshPart(
    Effect*, const GeoSphereParams&, const Matrix4& mat);
AZER_EXPORT MeshPartPtr CreateSphereFrameMeshPart(
    Effect*, const GeoSphereParams&, const Matrix4& mat);

// box
AZER_EXPORT EntityPtr CreateBoxEntity(VertexDesc* desc, const Matrix4& mat);
AZER_EXPORT EntityPtr CreateBoxFrameEntity(VertexDesc*, const Matrix4& mat);
AZER_EXPORT MeshPartPtr CreateBoxMeshPart(Effect* desc, const Matrix4& mat);
AZER_EXPORT MeshPartPtr CreateBoxFrameMeshPart(Effect* e, const Matrix4& mat);

// plane
struct AZER_EXPORT GeoPlaneParams {
  GeoPlaneParams() : row(10), column(10), row_width(1.0f), column_width(1.0f) {}
  int32 row;
  int32 column;
  float row_width;
  float column_width;
};
AZER_EXPORT EntityPtr CreatePlaneEntity(
    VertexDesc* desc, const GeoPlaneParams& params, const Matrix4& mat);
AZER_EXPORT EntityPtr CreatePlaneFrameEntity(
    VertexDesc* desc, const GeoPlaneParams& params, const Matrix4& mat);

AZER_EXPORT MeshPartPtr CreatePlaneMeshPart(
    Effect*, const GeoPlaneParams& params, const Matrix4& mat);
AZER_EXPORT MeshPartPtr CreatePlaneFrameMeshPart(
    Effect*, const GeoPlaneParams&, const Matrix4& mat);

// round
AZER_EXPORT EntityPtr CreateRoundEntity(
    VertexDesc* desc, float radius, int32 slice, const Matrix4& mat);
AZER_EXPORT EntityPtr CreateCircleEntity(
    VertexDesc* desc, float radius, int32 slice, const Matrix4& mat);
AZER_EXPORT MeshPartPtr CreateRoundMeshPart(
    Effect* e, float radius, int32 slice, const Matrix4& mat);
AZER_EXPORT MeshPartPtr CreateCircleMeshPart(
    Effect* e, float radius, int32 slice, const Matrix4& mat);

//
struct AZER_EXPORT GeoConeParams {
  GeoConeParams() : height(1.0f), radius(1.0f), slice(64) {}
  float height;
  float radius;
  int slice;
};
AZER_EXPORT EntityDataPtr CreateTaperEntity(
    VertexDesc* desc, const GeoConeParams& params, const Matrix4& mat);
AZER_EXPORT EntityDataPtr CreateConeEntity(
    VertexDesc* desc, const GeoConeParams& params, const Matrix4& mat);

struct AZER_EXPORT GeoBarrelParams {
  GeoBarrelParams() : top_radius(1.0f), bottom_radius(1.0f), height(1.0f), 
                      slice(64), stack(64) {}
  float top_radius;
  float bottom_radius;
  float height;
  int32 slice;
  int32 stack;
};

AZER_EXPORT EntityPtr CreateBarrelEntity(
    VertexDesc* desc, const GeoBarrelParams& params, const Matrix4& mat);
AZER_EXPORT MeshPartPtr CreateBarrelMeshPart(
    Effect* e, const GeoBarrelParams& params, const Matrix4& mat);
AZER_EXPORT MeshPartPtr CreateCylinderMeshPart(
    Effect* e, const GeoBarrelParams& params, const Matrix4& mat);

struct AZER_EXPORT GeoTourParams {
  GeoTourParams() : radius1(1.0f), radius2(1.0f), slice(64), stack(64) {}
  float radius1;
  float radius2;
  int32 slice;
  int32 stack;
};
AZER_EXPORT EntityPtr CreateTourEntity(
    VertexDesc* desc, const GeoTourParams& params, const Matrix4& mat);
AZER_EXPORT MeshPartPtr CreateTourMeshPart(
    Effect* e, const GeoTourParams& params, const Matrix4& mat);

struct AZER_EXPORT GeoAxisParams {
  GeoAxisParams() : axis_length(0.8f), axis_radius(0.04f), 
                    cone_radius(0.06f), cone_height(0.2f), slice(64) {}
  float axis_length;
  float axis_radius;
  float cone_radius;
  float cone_height;
  int32 slice;
};

AZER_EXPORT MeshPartPtr CreateAxisMeshPart(
    Effect* e, const GeoAxisParams& params, const Matrix4& transform);
AZER_EXPORT MeshPartPtr CreateLineAxisMeshPart(
    Effect* e, const GeoAxisParams& params, const Matrix4& transform);

AZER_EXPORT EntityPtr CreateGeoPointsList(PrimitiveTopology primitive,
                                          const Vector3* points, int32 count,
                                          VertexDesc* desc, const Matrix4& mat);

// util
AZER_EXPORT void CalcIndexedTriangleNormal(VertexData* vbd, IndicesData* idata);
AZER_EXPORT void CalcTriangleNormal(VertexData* vbd, int* indices);
}  // namespace azer
