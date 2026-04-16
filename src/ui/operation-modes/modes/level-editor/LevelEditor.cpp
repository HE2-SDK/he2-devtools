#include "LevelEditor.h"
#include "SetObjectList.h"
#include "ObjectDataInspector.h"
#include "ObjectLibrary.h"
#include "ArrayTool.h"
#include "Actions.h"
#include "Behaviors.h"

namespace ui::operation_modes::modes::level_editor {
	using namespace hh::fnd;
	using namespace hh::game;

	LevelEditor::LevelEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host) : OperationMode{ allocator, host } {
		AddPanel<SetObjectList>();
		AddPanel<ObjectDataInspector>();
		AddPanel<ObjectLibrary>();
		AddPanel<ArrayTool>();
		AddBehavior<SelectionBehavior>();
		AddBehavior<SelectionAabbBehavior>();
		AddBehavior<SelectionTransformationBehavior>();
		AddBehavior<SelectionVisual3DBehavior>();
		AddBehavior<GizmoBehavior>();
		AddBehavior<MousePickingBehavior>();
		AddBehavior<SelectionMousePickingBehavior>();
		AddBehavior<DeleteBehavior>();
		AddBehavior<PlacementBehavior>();
		AddBehavior<ClipboardBehavior>();
		AddBehavior<ObjectLocationVisual3DBehavior>();
		AddBehavior<GroundContextMenuBehavior>();
		AddBehavior<DebugCommentsVisualBehavior>();
		AddBehavior<DragDropPlacementBehavior>();

		auto* gameManager = GameManager::GetInstance();

		gameManager->AddListener(this);

		if (auto* objWorld = gameManager->GetService<ObjectWorld>())
			objWorld->AddWorldListener(this);
	}

	LevelEditor::~LevelEditor() {

		auto* gameManager = GameManager::GetInstance();

		gameManager->RemoveListener(this);

		if (auto* objWorld = gameManager->GetService<ObjectWorld>())
			objWorld->RemoveWorldListener(this);
	} 

	void LevelEditor::ProcessAction(const ActionBase& action) {
		OperationMode::ProcessAction(action);

		switch (action.id) {
		case SelectionTransformationBehavior<Context>::SelectionTransformChangedAction::id: {
			auto& context = GetContext();
			auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

			context.RecalculateDependentTransforms(selection);

			if (selection.size() == 1)
				context.NotifyUpdatedObject(selection[0]);

			context.UpdatePaths();
			Dispatch(PathsUpdatedAction{});
			break;
		}
		case SelectionBehavior<Context>::SelectionChangedAction::id: {
			auto& changes = static_cast<const SelectionBehavior<Context>::SelectionChangedAction&>(action).payload;

			if (changes.previousSelection.size() == 1 && (changes.currentSelection.size() != 1 || changes.previousSelection[0] != changes.currentSelection[0]))
				GetContext().NotifyDeselectedObject(changes.previousSelection[0]);
			if (changes.currentSelection.size() == 1 && (changes.previousSelection.size() != 1 || changes.previousSelection[0] != changes.currentSelection[0]))
				GetContext().NotifySelectedObject(changes.currentSelection[0]);

			break;
		}
		case SetFocusedChunkAction::id: {
			auto* chunk = static_cast<const SetFocusedChunkAction&>(action).payload;

			if (GetContext().GetFocusedChunk() == chunk)
				return;

			ClearChunkReferences();
			GetContext().SetFocusedChunk(chunk);
			Dispatch(FocusedChunkChangedAction{});
			break;
		};
		case ChangingParametersAction::id:
			GetContext().ReloadActiveObjectParameters(GetBehavior<SelectionBehavior<Context>>()->GetSelection()[0]);
			break;
		case StopChangingParametersAction::id:
			GetContext().RespawnActiveObject(GetBehavior<SelectionBehavior<Context>>()->GetSelection()[0]);
			break;
		case FocusObjectDataAction::id:
			GetBehavior<SelectionBehavior<Context>>()->Select(static_cast<const FocusObjectDataAction&>(action).payload);
			break;
		case SetObjectClassToPlaceAction::id:
			GetContext().SetObjectClassToPlace(static_cast<const SetObjectClassToPlaceAction&>(action).payload);
			Dispatch(ObjectClassToPlaceChangedAction{});
			break;
		case SetLayerEnabledAction::id: {
			auto& payload = static_cast<const SetLayerEnabledAction&>(action).payload;

			GetContext().SetLayerEnabled(payload.layerName, payload.enabled);
			break;
		}
		}
	}

	void LevelEditor::GameServiceAddedCallback(GameService* service) {
		if (service->staticClass == ObjectWorld::GetClass()) {
			auto* objWorld = static_cast<ObjectWorld*>(service);
			objWorld->AddWorldListener(this);
		}
	}

	void LevelEditor::GameServiceRemovedCallback(GameService* service) {
		if (service->staticClass == ObjectWorld::GetClass()) {
			auto* objWorld = static_cast<ObjectWorld*>(service);
			objWorld->RemoveWorldListener(this);
		}
	}

	void LevelEditor::WorldChunkRemovedCallback(ObjectWorldChunk* chunk)
	{
		if (GetContext().GetFocusedChunk() != chunk)
			return;

		ClearChunkReferences();
		GetContext().ReleaseChunk();
		Dispatch(FocusedChunkChangedAction{});
	}

	void LevelEditor::ClearChunkReferences()
	{
		GetBehavior<ClipboardBehavior<Context>>()->Clear();
		GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
		HandlePendingActions();
	}
}
