#pragma once
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::scene_editor
{
	class SceneEditor : 
		public OperationMode<Context>
	{
	public:
		SceneEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host);
		~SceneEditor();
	};
}
