#pragma once
#include <utilities/CompatibleObject.h>
#include "renderables/Colliders.h"
#include "renderables/GOCVisualDebugDraws.h"
#include "renderables/OcclusionCapsules.h"
#include "renderables/Paths.h"
#include "renderables/Bones.h"
#ifndef DEVTOOLS_TARGET_SDK_wars
#include "renderables/PhysicalAnimation.h"
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "renderables/Light.h"
#endif

namespace devtools::debug_rendering {
	class DebugRenderingSystem : public CompatibleObject {
	public:
		renderables::Colliders collidersRenderable{};
		renderables::GOCVisualDebugDraws gocVisualDebugDrawsRenderable{ GetAllocator() };
		renderables::OcclusionCapsules occlusionCapsulesRenderable{};
		renderables::Paths pathsRenderable{};
		renderables::Bones bonesRenderable{};
#ifndef DEVTOOLS_TARGET_SDK_wars
		renderables::PhysicalAnimation physicalAnimationRenderable{};
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
		renderables::Light lightRenderable{};
#endif

		static hh::fnd::Reference<DebugRenderingSystem> instance;

		using CompatibleObject::CompatibleObject;

		static void Init(csl::fnd::IAllocator* allocator);
		static void Deinit();
	};
}
