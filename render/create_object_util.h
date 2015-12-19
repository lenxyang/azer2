#pragma once

#include "azer/render/render_system.h"

namespace azer {
AZER_EXPORT IndicesBufferPtr CreateIndicesBufferWithDefaultOpt(IndicesData* data);
AZER_EXPORT VertexBufferPtr CreateVertexBufferWithDefaultOpt(SlotVertexData* data);
AZER_EXPORT VertexBufferGroupPtr CreateVertexBufferGroup(
    const VertexBuffer::Options& opt, VertexData* vdata);
AZER_EXPORT VertexBufferGroupPtr CreateVertexBufferGroupWithDefaultOpt(
    VertexData* vdata);

// create texture
AZER_EXPORT TexturePtr CreateShaderTexture(const Image* image);
}  // namespace azer
