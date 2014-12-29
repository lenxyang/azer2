#include "azer/render_system/d3d11/fence.h"

#include "azer/render_system/d3d11/renderer.h"
#include "azer/render_system/d3d11/util.h"
#include "base/logging.h"

namespace azer {
namespace d3d11 {


bool D3DFence::Begin(bool current) {
  DCHECK(NULL == query_);
  D3D11_QUERY_DESC desc;
  desc.Query = D3D11_QUERY_EVENT;
  desc.MiscFlags = 0;
  
  HRESULT hr = S_OK;
  ID3D11DeviceContext* d3d_context = renderer->GetContext();
  hr = d3d_context->CreateQuery(desc, &query_);
  if (FAILED(hr)) {
    return false;
  } 
  return true;
}

bool D3DFence::End() {
  DCHECK(NULL != query_);
  
  ID3D11DeviceContext* d3d_context = renderer->GetContext();
  d3d_context->GetData(query, NULL, 0, 0);
  return true;
}
}  // namespace d3d11
}  // namespace azer
