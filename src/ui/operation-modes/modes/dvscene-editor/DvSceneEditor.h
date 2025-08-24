#pragma once
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::dvscene_editor
{
	class DvSceneEditor : 
		public OperationMode<Context>,
		public hh::game::GameManagerListener
	{
	public:
		DvSceneEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host);
		~DvSceneEditor();

		bool listenerAdded = false;

		virtual void GameObjectRemovedCallback(hh::game::GameManager* gameManager, hh::game::GameObject* gameObject) override;
	};
}
