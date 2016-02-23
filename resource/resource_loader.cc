#include "azer/resource/resource_loader.h"

#include "base/logging.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/base/file_system.h"
#include "azer/render/blending.h"
#include "azer/resource/xml_util.h"
#include "azer/resource/effect_loader.h"
#include "azer/resource/mesh_loader.h"
#include "azer/resource/light_loader.h"
#include "azer/resource/vertex_desc_loader.h"
#include "azer/resource/scene_loader.h"

namespace azer {
namespace {
bool LoadContents(const ResPath& path, FileContents* contents, FileSystem* fs) {
  FilePtr file = fs->OpenFile(path);
  if (!file.get()) {
    LOG(ERROR) << "Failed to open file: " << path.fullpath();
    return false;
  }

  if (!file->PRead(0, -1, contents)) {
    LOG(ERROR) << "Failed to read file: " << path.fullpath();
    return false;
  }
  return true;
}
}

ResourceLoadContext::ResourceLoadContext() 
    : loader(NULL),
      file_system(NULL),
      effectlib(NULL),
      effect_adapter_context(NULL) {
}

ResourceLoader::ResourceLoader(ResourceLoadContext* ctx)
    : context_(*ctx) {
  context_.loader = this;
}

ResourceLoader::~ResourceLoader() {}
FileSystem* ResourceLoader::file_system() { return context_.file_system;}

void ResourceLoader::RegisterSpecialLoader(ResourceSpecialLoader* loader){
  auto iter = dict_.find(loader->GetLoaderName());
  if (iter == dict_.end()) {
    dict_.insert(std::make_pair(loader->GetLoaderName(), loader));
  } else {
    CHECK(false) << "Loader \"" << loader->GetLoaderName() << "\" has been inserted";
  }
}

ResourceSpecialLoader* ResourceLoader::GetLoader(ConfigNode *node) {
  for (auto iter = dict_.begin(); iter != dict_.end(); ++iter) {
    if (iter->second->CouldLoad(node)) {
      return iter->second.get();
    }
  }
  return NULL;
}

VariantResource ResourceLoader::Load(const ResPath& path) {
  FileContents contents;
  if (!LoadContents(path, &contents, file_system())) {
    LOG(ERROR) << "Failed to Load contents from file: " << path.fullpath();
    return VariantResource();
  }

  CHECK(path.IsAbsolutePath());
  ResourceLoadContext ctx = context_;
  ctx.path = path;
  ctx.loader = this;
  std::string strcontents((const char*)&contents.front(), contents.size());
  ConfigNodePtr croot = ConfigNode::InitFromXMLStr(strcontents);

  base::string16 nodepath = ::base::UTF8ToUTF16("//");
  nodepath.append(path.component_name().as_string());
  ConfigNodePtr cnode = croot->GetNodeFromPath(::base::UTF16ToUTF8(nodepath));
  CHECK(cnode.get()) << "cannot node: " << nodepath;
  ResourceSpecialLoader* loader = GetLoader(cnode);
  return loader->Load(cnode.get(), &ctx);
}
}  // namespace azer
