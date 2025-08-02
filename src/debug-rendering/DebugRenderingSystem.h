#pragma once
#include <utilities/CompatibleObject.h>
#include "renderables/Colliders.h"
#include "renderables/GOCVisualDebugDraws.h"
#include "renderables/OcclusionCapsules.h"
#include "renderables/Paths.h"
#include "renderables/Bones.h"

namespace devtools::debug_rendering {
	class DebugRenderingSystem : public CompatibleObject {
	public:
		renderables::Colliders collidersRenderable{};
		renderables::GOCVisualDebugDraws gocVisualDebugDrawsRenderable{ GetAllocator() };
		renderables::OcclusionCapsules occlusionCapsulesRenderable{};
		renderables::Paths pathsRenderable{};
		renderables::Bones bonesRenderable{};

		static hh::fnd::Reference<DebugRenderingSystem> instance;

		using CompatibleObject::CompatibleObject;

		static void Init(csl::fnd::IAllocator* allocator);
		static void Deinit();
	};
}
