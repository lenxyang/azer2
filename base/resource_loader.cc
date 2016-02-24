#include "azer/base/resource_loader.h"

#include "base/logging.h"
#include "base/strings/utf_string_conversions.h"
#include "azer/base/config_node.h"
#include "azer/base/file_system.h"

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

namespace {
bool Repath(const ResPath& path, ResPath* apath, ResourceLoadContext* ctx) {
  CHECK(!path.empty());
  CHECK(!ctx->path.empty());
  if (path.IsAbsolutePath()) {
    *apath = path;
  } else if (!path.component().empty() && path.filepath().empty()) {
    *apath = ResPath(ctx->path.filepath().as_string());
    CHECK(apath->Append(ResPath(path.component().as_string())));
  } else {
    *apath = ResPath(ctx->path.filepath().as_string());
    CHECK(apath->Append(ResPath(path.fullpath())));
  }
  return !apath->empty();
}
}  // namespace

VariantResource LoadResource(const ResPath& path, int type, ResourceLoadContext* ctx) {
  CHECK(!path.empty());
  ResPath npath;
  CHECK(Repath(path, &npath, ctx));
  VariantResource ret = ctx->loader->Load(npath);
  if (ret.retcode != 0) {
    LOG(ERROR) << "Load Effect failed for path: " << npath.fullpath();
    return VariantResource();
  }
  if (ret.type != type) {
    LOG(ERROR) << "Not Effect for path: " << npath.fullpath();
    return VariantResource();
  }

  return ret;
}

VariantResource LoadReferResource(const ConfigNode* node, ResourceLoadContext* ctx) {
  if (!node) {
    return VariantResource();
  }
  DCHECK(node->tagname() == "refer");
  ResPath path(::base::UTF8ToUTF16(node->GetAttr("path")));
  CHECK(!path.empty());
  int type = GetTypeFromString(node->GetAttr("type"));
  return LoadResource(path, type, ctx);
}

VariantResource LoadResource(const azer::ResPath& path, int type,
                             ResourceLoader* loader) {
  CHECK(path.IsAbsolutePath());
  ResourceLoadContext ctx;
  ctx.path = path;
  ctx.loader = loader;
  ctx.file_system = loader->file_system();
  return LoadResource(path, type, &ctx);
}

ConfigNode* GetTypedReferNode(const std::string& type_name,
                              const ConfigNode* parent) {
  ConfigNodes nodes = parent->GetTaggedChildren("refer");
  for (auto iter = nodes.begin(); iter != nodes.end(); ++iter) {
    if ((*iter)->GetAttr("type") == type_name) {
      return (*iter).get();
    }
  }

  return NULL;
}
}  // namespace azer
