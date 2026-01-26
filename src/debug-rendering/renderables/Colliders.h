#pragma once
#include <debug-rendering/DebugRenderable.h>
#include <utilities/ColliderFilter.h>

namespace devtools::debug_rendering::renderables {
	class Colliders : public OptionalDebugRenderable {
	public:
		bool enabled{};
		bool colliderFilters[32][32];
		//ColliderFilterList colliderFilterList;
#ifdef DEVTOOLS_TARGET_SDK_rangers
		bool meshEnabled{};
#endif

		virtual void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) override;
	};
}
