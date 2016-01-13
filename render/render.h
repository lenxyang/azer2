#pragma once

#include "azer/render/axis_aligned_box.h"
#include "azer/render/blending.h"
#include "azer/render/camera.h"
#include "azer/render/effect.h"
#include "azer/render/effect_params_adapter.h"
#include "azer/render/effect_params_provider.h"
#include "azer/render/mesh.h"
#include "azer/render/geometry/box_object.h"
#include "azer/render/geometry/circle_object.h"
#include "azer/render/geometry/cone_object.h"
#include "azer/render/geometry/cylinder_object.h"
#include "azer/render/geometry/frustum_object.h"
#include "azer/render/geometry/plane_object.h"
#include "azer/render/geometry/sphere_object.h"
#include "azer/render/geometry/square_trustum.h"
#include "azer/render/gpu_constants_table.h"
#include "azer/render/indices_buffer.h"
#include "azer/render/rasterizer_state.h"
#include "azer/render/render_system_enum.h"
#include "azer/render/render_target.h"
#include "azer/render/render_system.h"
#include "azer/render/renderer.h"
#include "azer/render/overlay.h"
#include "azer/render/technique.h"
#include "azer/render/vertex_buffer.h"
#include "azer/render/vertex_pack.h"
#include "azer/render/create_object_util.h"

#include "azer/render/scoped_render_target.h"
#include "azer/render/render_system_loader.h"
#include "azer/render/util/coordinate_grid.h"
