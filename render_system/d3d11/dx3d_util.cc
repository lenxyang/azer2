#include "azer/render_system/d3d11/dx3d_util.h"

#include <windows.h>
#include <tchar.h>
#include <comdef.h>

#include "azer/base/string.h"
#include "base/logging.h"
#include "base/strings/string16.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/render_system/d3d11/render_system.h"

namespace azer {
namespace d3d11 {
std::string HRMessage(uint32_t hr) {
  /*
  string16 msg;
  LPTSTR pvMsgBuf = NULL;
  if(FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM
                   | FORMAT_MESSAGE_FROM_SYSTEM
                   | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   (DWORD)hr,
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                   &pvMsgBuf, 0, NULL)) {
    // Do something with the error message.
    msg.append((LPTSTR)pvMsgBuf);
    LocalFree(pvMsgBuf);
  }
  */
  _com_error error(hr);

  ::base::string16 msg = error.ErrorMessage();
  std::string str(::base::UTF16ToUTF8(msg));
  return str;
}

void ReportDeviceObjects(D3DRenderSystem* rs) {
  ID3D11Debug * debug = NULL;
  ID3D11Device* device = rs->GetDevice();
  HRESULT hr = device->QueryInterface(__uuidof(ID3D11Debug),
                                      reinterpret_cast<void**>(&debug));
  if (SUCCEEDED(hr)) {
    debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    SAFE_RELEASE(device);
  }
}
}  // namespace d3d11
}
