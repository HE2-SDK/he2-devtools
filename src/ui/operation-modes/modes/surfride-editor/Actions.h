#pragma once

namespace ui::operation_modes::modes::surfride_editor {
	struct AddCastToLayerPayload {
		ucsl::resources::swif::swif_version::SRS_LAYER& layer;
		ucsl::resources::swif::swif_version::SRS_CASTNODE::Type type;
	};

	struct AddCastToCastPayload {
		ucsl::resources::swif::swif_version::SRS_CASTNODE& cast;
		ucsl::resources::swif::swif_version::SRS_CASTNODE::Type type;
	};

	using SetResourceAction = Action<ActionId::SURFRIDE_EDITOR_SET_RESOURCE, hh::ui::GOCSprite*>;
	using ResourceChangedAction = Action<ActionId::SURFRIDE_EDITOR_RESOURCE_CHANGED>;
	using SetFocusedSceneAction = Action<ActionId::SURFRIDE_EDITOR_SET_FOCUSED_SCENE, ucsl::resources::swif::swif_version::SRS_SCENE*>;
	using FocusedSceneChangedAction = Action<ActionId::SURFRIDE_EDITOR_FOCUSED_SCENE_CHANGED>;
	using AddCastToLayerAction = Action<ActionId::SURFRIDE_EDITOR_ADD_CAST_TO_LAYER, AddCastToLayerPayload>;
	using AddCastToCastAction = Action<ActionId::SURFRIDE_EDITOR_ADD_CAST_TO_CAST, AddCastToCastPayload>;
	using AddSceneAction = Action<ActionId::SURFRIDE_EDITOR_ADD_SCENE>;
	using RemoveCastAction = Action<ActionId::SURFRIDE_EDITOR_REMOVE_CAST, ucsl::resources::swif::swif_version::SRS_CASTNODE&>;
	using RemoveLayerAction = Action<ActionId::SURFRIDE_EDITOR_REMOVE_LAYER, ucsl::resources::swif::swif_version::SRS_LAYER&>;
	using RemoveSceneAction = Action<ActionId::SURFRIDE_EDITOR_REMOVE_SCENE, ucsl::resources::swif::swif_version::SRS_SCENE&>;
	using OpenTextureEditorAction = Action<ActionId::SURFRIDE_EDITOR_OPEN_TEXTURE_EDITOR>;
	using CriticalParametersChangedAction = Action<ActionId::SURFRIDE_EDITOR_CRITICAL_PARAMETERS_CHANGED>;
}
