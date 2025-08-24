#pragma once

#ifdef DEVTOOLS_TARGET_SDK_wars
typedef std::tuple<
    app::gfx::FxParamManager,
    hh::game::ObjectWorld,
    app::level::StageInfo,
    hh::game::CameraManager
> KnownResourceTypes;
#endif

#ifdef DEVTOOLS_TARGET_SDK_rangers
namespace heur::resources {
    class ResLevel { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResMasterLevel { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResEffect { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
                                      //class ResSoftBody { public: inline static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
                                      //class ResHelperBone { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
                                      //class ResOrcaData { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResAnimation { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
    class ResSkeleton { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
}

namespace app::gfx {
    class ResPointcloudModel { public: static const hh::fnd::ResourceTypeInfo* GetTypeInfo(); };
}

typedef std::tuple<
    app::gfx::FxParamManager,
    hh::game::ObjectWorld,
    app::level::StageInfo,
    app::level::LevelInfo,
    hh::game::CameraManager,
    app::camera::CameraService,
    app::game::GameModeResourceManager,
    app::trr::TerrainManager,
    heur::resources::ResLevel,
    heur::resources::ResMasterLevel,
    heur::resources::ResEffect,
    hh::pba::ResPhysicalSkeleton,
    heur::resources::ResAnimation,
    heur::resources::ResSkeleton,
    hh::dv::ResDvScene,
    app::gfx::ResPointcloudModel
> KnownResourceTypes;

#endif

const char* GetExtensionByTypeInfo(const hh::fnd::ResourceTypeInfo* typeInfo);
const hh::fnd::ResourceTypeInfo* GetTypeInfoByExtension(const char* extension);
