#include "azer/resource/effect_loader.h"

#include "base/logging.h"
#include "base/strings/string_util.h"
#include "base/strings/string_number_conversions.h"
// #include "base/strings/utf_string_conversions.h"
#include "azer/effect/effect_creator.h"
#include "azer/effect/effectlib.h"
#include "azer/render/blending.h"
#include "azer/render/technique.h"
#include "azer/resource/resource_util.h"

namespace azer {
using base::UTF8ToUTF16;
using base::UTF16ToUTF8;

const char EffectLoader::kSpecialLoaderName[] = "azer::EffectLoader";
EffectLoader::EffectLoader() {}
EffectLoader::~EffectLoader() {}

const char* EffectLoader::GetLoaderName() const {
  return kSpecialLoaderName;
}

namespace {
int GetStageFromName(const std::string& name) {
  if (name == "vertex") {
    return kVertexStage;
  } else if (name == "pixel") {
    return kPixelStage;
  } else if (name == "geometry") {
    return kGeometryStage;
  } else {
    CHECK(false) << "no supported stage \"" << name << "\"";
    return -1;
  }
}
}

VariantResource EffectLoader::Load(const ConfigNode* node, 
                                   ResourceLoadContext* ctx) {
  const std::string& type = node->GetAttr("type");
  if (type == "effectlib") {
    return LoadFromLib(node, ctx);
  } else if (type == "effect") {
    return LoadSource(node, ctx);
  } else {
    NOTREACHED();
    VariantResource resource;
    return resource;
  }
}

VariantResource EffectLoader::LoadFromLib(const ConfigNode* node, 
                                          ResourceLoadContext* ctx) {
  const std::string& name = node->GetAttr("name");
  VariantResource resource;
  resource.effect = ctx->effectlib->GetEffect(name);
  resource.type = kResTypeEffect;
  resource.retcode = 0;
  return resource;
}

VariantResource EffectLoader::LoadSource(const ConfigNode* node,
                                         ResourceLoadContext* ctx) {
  ResPath vertex_desc_path(UTF8ToUTF16(node->GetAttr("vertex_desc")));
  DCHECK(!vertex_desc_path.empty());
  VertexDescPtr vertex_desc = LoadVertexDesc(vertex_desc_path, ctx);
  DCHECK(vertex_desc.get()) << "vertex desc no specified on effect \""
                            << node->GetNodePath() << "\"";

  TechSource program(vertex_desc.get());
  ConfigNodes item = node->GetTaggedChildren("shader");
  FileContents contents;
  for (auto iter = item.begin(); iter != item.end(); ++iter) {
    contents.clear();
    ConfigNode* n = iter->get();
    ShaderInfo info;
    info.stage = GetStageFromName(n->GetAttr("stage"));
    info.entry = n->GetAttr("entry");
    info.version = n->GetAttr("version");
    info.path = n->GetAttr("path");
    ResPath respath(UTF8ToUTF16(info.path));
    if (!LoadFileContents(respath, &contents, ctx->file_system)) {
      LOG(ERROR) << "Failed to loader path: " << info.path;
      return VariantResource();
    }
    info.code = std::string((const char*)&contents.front(), contents.size());
    program.SetStage(info.stage, info);
  }

  EffectPtr effect = CreateEffectByName(node->GetAttr("effect_name"));
  if (!effect.get() || !effect->Init(program)) {
    LOG(ERROR) << "Failed to init effect \"" << node->GetAttr("effect_name") << "\"";
    return VariantResource();
  }

  VariantResource resource;
  resource.type = kResTypeEffect;
  resource.effect = effect;
  resource.retcode = 0;
  return resource;
}

bool EffectLoader::CouldLoad(ConfigNode* node) const {
  return node->tagname() == "effect";
}

}  // namespace azer
