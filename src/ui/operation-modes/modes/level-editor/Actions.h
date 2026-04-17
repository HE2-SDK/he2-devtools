#pragma once
#include <ui/Action.h>

namespace ui::operation_modes::modes::level_editor {
	struct SetLayerEnabledPayload {
		const char* layerName{};
		bool enabled{};
	};

	using ChangingParametersAction = Action<ActionId::LEVEL_EDITOR_CHANGING_PARAMETERS>;
	using StopChangingParametersAction = Action<ActionId::LEVEL_EDITOR_STOP_CHANGING_PARAMETERS>;
	using ObjectTransformChangedAction = Action<ActionId::LEVEL_EDITOR_OBJECT_TRANSFORM_CHANGED>;
	using SetFocusedChunkAction = Action<ActionId::LEVEL_EDITOR_SET_FOCUSED_CHUNK, hh::game::ObjectWorldChunk*>;
	using FocusedChunkChangedAction = Action<ActionId::LEVEL_EDITOR_FOCUSED_CHUNK_CHANGED>;
	using PathsUpdatedAction = Action<ActionId::LEVEL_EDITOR_PATHS_UPDATED>;
	using OpenArrayToolAction = Action<ActionId::LEVEL_EDITOR_OPEN_ARRAY_TOOL>;
	using SetObjectClassToPlaceAction = Action<ActionId::LEVEL_EDITOR_SET_OBJECT_CLASS_TO_PLACE, const hh::game::GameObjectClass*>;
	using ObjectClassToPlaceChangedAction = Action<ActionId::LEVEL_EDITOR_OBJECT_CLASS_TO_PLACE_CHANGED>;
	using SetLayerEnabledAction = Action<ActionId::LEVEL_EDITOR_SET_LAYER_ENABLED, SetLayerEnabledPayload>;
	using CloneSelectionAction = Action<ActionId::LEVEL_EDITOR_CLONE_SELECTION>;
}
