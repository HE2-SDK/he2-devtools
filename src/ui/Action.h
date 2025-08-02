#pragma once
#include <utilities/CompatibleObject.h>

#ifdef DELETE
#undef DELETE
#endif

enum class ActionId {
	SELECT,
	DESELECT,
	SELECT_ALL,
	DESELECT_ALL,
	SELECTION_CHANGED,
	SET_SELECTION_TRANSFORM,
	SELECTION_TRANSFORM_CHANGED,
	OBJECTS_PICKED,
	LOCATION_PICKED,
	CUT,
	COPY,
	PASTE,
	CHANGE_COORDINATE_SYSTEM,
	TRANSFORM_MODE,
	TRANSLATE_MODE,
	NO_TRANSLATE_MODE,
	ROTATE_MODE,
	NO_ROTATE_MODE,
	SCALE_MODE,
	NO_SCALE_MODE,
	ONLY_X_AXIS,
	LOCK_X_AXIS,
	ONLY_Y_AXIS,
	LOCK_Y_AXIS,
	ONLY_Z_AXIS,
	LOCK_Z_AXIS,
	SCENE_CHANGED,
	DELETE,
	OBJECT_PLACED,
	TOGGLE_PLACE_MODE,
	LEVEL_EDITOR_OBJECT_TRANSFORM_CHANGED,
	LEVEL_EDITOR_CHANGING_PARAMETERS,
	LEVEL_EDITOR_STOP_CHANGING_PARAMETERS,
	LEVEL_EDITOR_SET_FOCUSED_CHUNK,
	LEVEL_EDITOR_FOCUSED_CHUNK_CHANGED,
	LEVEL_EDITOR_PATHS_UPDATED,
	LEVEL_EDITOR_OPEN_ARRAY_TOOL,
	LEVEL_EDITOR_SET_OBJECT_CLASS_TO_PLACE,
	LEVEL_EDITOR_OBJECT_CLASS_TO_PLACE_CHANGED,
	LEVEL_EDITOR_SET_LAYER_ENABLED,
	FOCUS_GAME_OBJECT,
	FOCUS_OBJECT_DATA,
	EXPORT_RESOURCE,
	SURFRIDE_EDITOR_SET_RESOURCE,
	SURFRIDE_EDITOR_RESOURCE_CHANGED,
	SURFRIDE_EDITOR_SET_FOCUSED_SCENE,
	SURFRIDE_EDITOR_FOCUSED_SCENE_CHANGED,
	SURFRIDE_EDITOR_ADD_CAST_TO_LAYER,
	SURFRIDE_EDITOR_ADD_CAST_TO_CAST,
	SURFRIDE_EDITOR_ADD_SCENE,
	SURFRIDE_EDITOR_REMOVE_CAST,
	SURFRIDE_EDITOR_REMOVE_LAYER,
	SURFRIDE_EDITOR_REMOVE_SCENE,
	SURFRIDE_EDITOR_OPEN_TEXTURE_EDITOR,
	SURFRIDE_EDITOR_CRITICAL_PARAMETERS_CHANGED,
};

struct ActionBase;
struct AsyncActionBase : CompatibleObject {
	using CompatibleObject::CompatibleObject;

	virtual ~AsyncActionBase() = default;
	virtual ActionBase& GetAction() = 0;
};

template<typename A>
struct AsyncAction : AsyncActionBase {
	A action;

	AsyncAction(csl::fnd::IAllocator* allocator, const A& action) : AsyncActionBase{ allocator }, action{ action } {}

	//AsyncAction(csl::fnd::IAllocator* allocator, A&& action) : AsyncActionBase{ allocator }, action{ std::move(action) } {}

	virtual ActionBase& GetAction() override { return action; }
};

struct ActionBase {
	ActionId id;

	ActionBase(ActionId id);
	virtual ~ActionBase() = default;
	virtual AsyncActionBase* CreateAsync(csl::fnd::IAllocator* allocator) const &;
	//virtual AsyncActionBase* CreateAsync(csl::fnd::IAllocator* allocator) &&;
};

struct EmptyActionPayload {};

template<ActionId actionId, typename Payload = EmptyActionPayload>
struct Action : ActionBase {
	typedef Payload PayloadType;

	static constexpr ActionId id = actionId;

	Payload payload;

	explicit Action() : ActionBase{ actionId }, payload{} {}
	explicit Action(const std::remove_reference_t<Payload>& payload) : ActionBase{ actionId }, payload{ payload } {}
	explicit Action(std::remove_reference_t<Payload>& payload) : ActionBase{ actionId }, payload{ payload } {}
	explicit Action(std::remove_reference_t<Payload>&& payload) : ActionBase{ actionId }, payload{ std::move(payload) } {}
	Action(const Action<actionId, Payload>& other) : ActionBase{ other.id }, payload{ other.payload } {}
	Action(Action<actionId, Payload>&& other) : ActionBase{ other.id }, payload{ std::move(other.payload) } {}

	virtual AsyncActionBase* CreateAsync(csl::fnd::IAllocator* allocator) const & override {
		return new (allocator) AsyncAction<Action<actionId, Payload>>{ allocator, *this };
	}

	//virtual AsyncActionBase* CreateAsync(csl::fnd::IAllocator* allocator) && override {
	//	return new (allocator) AsyncAction<Action<actionId, Payload>>{ allocator, std::move(*this) };
	//}
};

class ActionDispatcher {
protected:
	void Dispatch(const ActionBase& action);
};

class ActionProcessor {
public:
	virtual void ProcessAction(const ActionBase& action);
};

// This should become a SceneEditing behavior and Place/Delete should use it to edit the scene.
using SceneChangedAction = Action<ActionId::SCENE_CHANGED>;
using FocusGameObjectAction = Action<ActionId::FOCUS_GAME_OBJECT, hh::game::GameObject*>;
using FocusObjectDataAction = Action<ActionId::FOCUS_OBJECT_DATA, hh::game::ObjectData*>;
using ExportResourceAction = Action<ActionId::EXPORT_RESOURCE>;
