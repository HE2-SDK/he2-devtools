#pragma once
#include <debug-rendering/DebugRenderable.h>

namespace devtools::debug_rendering::renderables {
	class Light : public OptionalDebugRenderable {
	public:
		bool enabled{};

		virtual void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) override;
		void RenderSpotlight(hh::gfnd::DrawContext& ctx, const hh::fnd::WorldPosition& worldPos, const float attenuationRadius, const float coneAngle, const csl::ut::Color8 lightColor);
	};
}
