#pragma once

#include <imgui.h>
#include <ui/Desktop.h>
#include <ui/common/Textures.h>

//struct IconUV {
//    ImWchar glyph;
//    float x;
//    float y;
//    float w;
//    float h;
//};
//
//template<typename Id, size_t Count>
//struct IconAtlas {
//    typedef Id Id_t;
//    static constexpr size_t count = Count;
//
//    IconUV uvs[Count];
//};
//
//enum class IconId {
//    TEXTURE,
//    LIGHTING,
//    SPRITE,
//    AI,
//    INPUT,
//    UI,
//    TRANSFORM,
//    PHYSICS,
//    ANIMATION,
//    GEOMETRY,
//    COLLISION,
//    SOUND,
//    PHOTO,
//    CAMERA,
//    STATE,
//    PARAMETER,
//    OBJECT,
//    SCRIPTING,
//    EVENT,
//};
//
//constexpr IconAtlas<IconId, 19> componentIcons = { 67, 84, "angryzor_devtools_component_icons", {
//    { U'\U00010000', 0, 0, 16, 16 },
//    { U'\U00010001', 17, 0, 16, 16 },
//    { U'\U00010002', 34, 0, 16, 16 },
//    { U'\U00010003', 51, 0, 16, 16 },
//    { U'\U00010004', 0, 17, 16, 16 },
//    { U'\U00010005', 17, 17, 16, 16 },
//    { U'\U00010006', 34, 17, 16, 16 },
//    { U'\U00010007', 51, 17, 16, 16 },
//    { U'\U00010008', 0, 34, 16, 16 },
//    { U'\U00010009', 17, 34, 16, 16 },
//    { U'\U00010010', 34, 34, 16, 16 },
//    { U'\U00010011', 51, 34, 16, 16 },
//    { U'\U00010012', 0, 51, 16, 16 },
//    { U'\U00010013', 17, 51, 16, 16 },
//    { U'\U00010014', 34, 51, 16, 16 },
//    { U'\U00010015', 51, 51, 16, 16 },
//    { U'\U00010016', 0, 68, 16, 16 },
//    { U'\U00010017', 17, 68, 16, 16 },
//    { U'\U00010018', 34, 68, 16, 16 },
//} };

struct ImgIcon {
    ImTextureID texId{};
    ImVec2 uv1{};
    ImVec2 uv2{};
    ImVec2 size{};
};

struct IconMap {
    ImWchar glyph;
    const char* filename;
};

enum class IconId {
    RESOURCE_SURFRIDE_PROJECT,
    RESOURCE_ATOM_CONFIG,
    RESOURCE_ATOM_PLAYLIST,
    RESOURCE_LIGHTING,
    RESOURCE_MATERIAL,
    RESOURCE_TERRAIN_MODEL,
    RESOURCE_MODEL,
    RESOURCE_COMPUTE_SHADER,
    RESOURCE_FRAGMENT_SHADER,
    RESOURCE_VERTEX_SHADER,
    RESOURCE_TEXTURE,
    RESOURCE_LEVEL,
    RESOURCE_MASTER_LEVEL,
    RESOURCE_BITMAP_FONT,
    RESOURCE_SCALABLE_FONT,
    RESOURCE_REFLECTION,
    RESOURCE_PACKFILE_LEVELS,
    RESOURCE_PACKFILE,
    RESOURCE_PARTICLESYSTEM,
    RESOURCE_PHYSICS_BONE,
    RESOURCE_SOFTBODY,
    RESOURCE_HELPER_BONE,
    RESOURCE_ORCA,
    RESOURCE_UI,
    RESOURCE_ANIMATION,
    RESOURCE_SKELETON,
    RESOURCE_DVSCENE,
    COMPONENT_TEXTURE,
    COMPONENT_LIGHTING,
    COMPONENT_SPRITE,
    COMPONENT_AI,
    COMPONENT_INPUT,
    COMPONENT_UI,
    COMPONENT_TRANSFORM,
    COMPONENT_PHYSICS,
    COMPONENT_ANIMATION,
    COMPONENT_GEOMETRY,
    COMPONENT_COLLISION,
    COMPONENT_SOUND,
    COMPONENT_PHOTO_CAMERA,
    COMPONENT_CAMERA,
    COMPONENT_STATE_MACHINE,
    COMPONENT_PARAMETER,
    COMPONENT_OBJECT,
    COMPONENT_SCRIPTING,
    COMPONENT_EVENT,
    COMPONENT_PATH,
    COMPONENT_POSTURE,
    COMPONENT_ENEMY,
    REORDERABLE,
};

constexpr IconMap icons[] = {
    { u'\uE000', "resources/surfride-project" },
    { u'\uE001', "resources/atom-config" },
    { u'\uE002', "resources/atom-playlist" },
    { u'\uE003', "resources/lighting" },
    { u'\uE004', "resources/material" },
    { u'\uE005', "resources/terrain-model" },
    { u'\uE006', "resources/model" },
    { u'\uE007', "resources/compute-shader" },
    { u'\uE008', "resources/fragment-shader" },
    { u'\uE009', "resources/vertex-shader" },
    { u'\uE010', "resources/texture" },
    { u'\uE011', "resources/level" },
    { u'\uE012', "resources/master-level" },
    { u'\uE013', "resources/bitmap-font" },
    { u'\uE014', "resources/scalable-font" },
    { u'\uE015', "resources/reflection" },
    { u'\uE016', "resources/packfile-levels" },
    { u'\uE017', "resources/packfile" },
    { u'\uE018', "resources/particlesystem" },
    { u'\uE019', "resources/physics-bone" },
    { u'\uE020', "resources/softbody" },
    { u'\uE021', "resources/helper-bone" },
    { u'\uE022', "resources/orca" },
    { u'\uE023', "resources/ui" },
    { u'\uE024', "resources/animation" },
    { u'\uE025', "resources/skeleton" },
    { u'\uE026', "resources/dvscene" },
    { u'\uE028', "components/texture" },
    { u'\uE029', "components/lighting" },
    { u'\uE030', "components/sprite" },
    { u'\uE031', "components/ai" },
    { u'\uE032', "components/input" },
    { u'\uE033', "components/ui" },
    { u'\uE034', "components/transform" },
    { u'\uE035', "components/physics" },
    { u'\uE036', "components/animation" },
    { u'\uE037', "components/geometry" },
    { u'\uE038', "components/collision" },
    { u'\uE039', "components/sound" },
    { u'\uE040', "components/photo-camera" },
    { u'\uE041', "components/camera" },
    { u'\uE042', "components/state-machine" },
    { u'\uE043', "components/parameter" },
    { u'\uE044', "components/object" },
    { u'\uE045', "components/scripting" },
    { u'\uE046', "components/event" },
    { u'\uE047', "components/path" },
    { u'\uE048', "components/posture" },
    { u'\uE049', "components/enemy" },
    { u'\uE050', "reorderable" },
};

void RegisterIconGlyphs(ImFont* font);
void AddIconTextures();

//template<typename Atlas>
//void RenderIcon(const Atlas& atlas, typename Atlas::Id_t id, ImVec2 size) {
//    auto& uv = atlas.uvs[static_cast<size_t>(id)];
//    auto packFile = hh::fnd::ResourceManager::GetInstance()->GetResource<hh::fnd::Packfile>("devtools");
//    auto texture = packFile->GetResourceByName<hh::gfnd::ResTexture>(atlas.atlasFile);
//
//    ImGui::Image(GetTextureIDFromResTexture(texture), size, ImVec2(uv.x / atlas.width, uv.y / atlas.height), ImVec2((uv.x + uv.w) / atlas.width, (uv.y + uv.h) / atlas.height));
//}
//
//template<typename Atlas>
//void RenderIcon(const Atlas& atlas, typename Atlas::Id_t id) {
//    auto& uv = atlas.uvs[static_cast<size_t>(id)];
//    RenderIcon(atlas, id, ImVec2(uv.w, uv.h));
//}

const char* GetIconGlyph(IconId id);
ImgIcon GetIconAsImage(IconId id);
