#include "azer/render/create_object_util.h"

#include "azer/render/texture.h"
#include "azer/render/vertex_buffer.h"

namespace azer {
IndicesBufferPtr CreateIndicesBufferWithDefaultOpt(IndicesData* data) {
  RenderSystem* rs = RenderSystem::Current();
  return rs->CreateIndicesBuffer(IndicesBuffer::Options(), data);
}

VertexBufferPtr CreateVertexBufferWithDefaultOpt(SlotVertexData* data) {
  RenderSystem* rs = RenderSystem::Current();
  return rs->CreateVertexBuffer(VertexBuffer::Options(), data);
}

VertexBufferGroupPtr CreateVertexBufferGroup(
    const VertexBuffer::Options& opt, VertexData* vdata) {
  RenderSystem* rs = RenderSystem::Current();
  VertexBufferGroupPtr group(rs->CreateVertexBufferGroup(vdata->vertex_desc()));
  int32 slot_count = vdata->vertex_desc()->slot_count();
  for (int32 i = 0; i < slot_count; ++i) {
    SlotVertexData* slot_data = vdata->vertex_data_at(i);
    VertexBufferPtr vb = rs->CreateVertexBuffer(opt, slot_data);
    group->add_vertex_buffer(vb.get());
  }
  return group;
}

VertexBufferGroupPtr CreateVertexBufferGroupWithDefaultOpt(
    VertexData* vdata) {
  return CreateVertexBufferGroup(VertexBuffer::Options(), vdata);
}

// create texture
TexturePtr CreateShaderTexture(const Image* image) {
  return TexturePtr();
}
}  // namespace azer
