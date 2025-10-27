#pragma once
#include <debug-rendering/DebugRenderable.h>
#include <utilities/CompatibleObject.h>

namespace devtools::debug_rendering::renderables {
	class GOCMyVisualDebugDraw;
	class GOCVisualDebugDraws : public CompatibleObject, public OptionalDebugRenderable {
		csl::ut::MoveArray<GOCMyVisualDebugDraw*> gocs{ GetAllocator() };

		friend class GOCMyVisualDebugDraw;

		void AddGOC(GOCMyVisualDebugDraw* goc);
		void RemoveGOC(GOCMyVisualDebugDraw* goc);

	public:
		bool enabled{};
		uint8_t opacity{ 80 };

		static GOCVisualDebugDraws* instance;

		GOCVisualDebugDraws(csl::fnd::IAllocator* allocator);
		~GOCVisualDebugDraws();

		virtual void RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) override;

		static void InstallHooks();
		static void RegisterClasses();
	};
}
