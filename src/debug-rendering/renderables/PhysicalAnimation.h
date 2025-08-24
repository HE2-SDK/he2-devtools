#pragma once
#ifndef DEVTOOLS_TARGET_SDK_wars
#include <debug-rendering/DebugRenderable.h>

namespace devtools::debug_rendering::renderables {
	class PhysicalAnimation : public OptionalDebugRenderable {
	public:
		bool enabled{};

		virtual void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) override;
		virtual void RenderImGuiDebugVisuals(const ImGuiDrawContext& ctx) override;
	};
}
#endif
