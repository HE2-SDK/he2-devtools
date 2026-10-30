#pragma once
#include <debug-rendering/DebugRenderable.h>

namespace devtools::debug_rendering::renderables {
	class NavMesh : public OptionalDebugRenderable {
	public:
		bool enabled{};

		virtual void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) override;
	};
}
