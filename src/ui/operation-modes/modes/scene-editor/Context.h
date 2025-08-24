#pragma once
#include <utilities/CompatibleObject.h>

namespace ui::operation_modes::modes::scene_editor {
	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;

		hh::scene::SceneControl* sceneCtrl{};

		Context(csl::fnd::IAllocator* allocator);
	};
}
